
#include "gps_thread.h"

Thread *_gpsThread;

TinyGPS *_gps;

// status flag to indicate if the GPS is information is ready
bool _gpsReady = false;

// Structure to hold processed GPS information
gps_info_t *_gps_info;

// For the distance calculation
bool gps_fix_first = true;
float gps_last_lon = 0, gps_last_lat = 0;

/* convert long integer from TinyGPS to float WGS84 degrees */
float get_wgs84_coordinate(unsigned long val)
{
  double result = 0.0;
  result = val/10000000.0;
  result = ((result-(int)result)/60.0)*100 + (int)result;
  return (float)result;
}

// Update GPS info from new sentence
void gpsInfoUpdate()
{
  SINGLE_THREADED_BLOCK() // we don't want context switching here
  {
    // get GPS date
    _gps->crack_datetime(
        &(_gps_info->year), 
        &(_gps_info->month), 
        &(_gps_info->day), 
        &(_gps_info->hour), 
        &(_gps_info->minute), 
        &(_gps_info->second), 
        &(_gps_info->hundredths), 
        &(_gps_info->age));

    if (TinyGPS::GPS_INVALID_AGE == _gps_info->age) 
    {
      _gps_info->year = 2012, _gps_info->month = 0, _gps_info->day = 0, 
      _gps_info->hour = 0, _gps_info->minute = 0, _gps_info->second = 0, 
      _gps_info->hundredths = 0;
    }

    // get GPS position, altitude and speed
    _gps->get_position(&(_gps_info->x), &(_gps_info->y), &(_gps_info->age));
    if (!_gps->status()) {
      _gps_info->gps_status = GPS_VOID;
    } else {
      _gps_info->gps_status = GPS_AVAILABLE;
    }
    _gps_info->faltitude = _gps->f_altitude();
    _gps_info->fspeed = _gps->f_speed_kmph();
    _gps_info->nbsat = _gps->satellites();
    _gps_info->precision = _gps->hdop();

    _gps_info->NS = 'N';
    _gps_info->WE = 'E';
    if (_gps_info->x < 0) { _gps_info->NS = 'S'; _gps_info->x = -_gps_info->x;}
    if (_gps_info->y < 0) { _gps_info->WE = 'W'; _gps_info->y = -_gps_info->y;}

    // compute distance
    if (_gps->status()) {
      int trigger_dist = 50;
      float flat = get_wgs84_coordinate(_gps_info->x);
      float flon = get_wgs84_coordinate(_gps_info->y);

      // if(_gps_info->fspeed > 5)
      //   // fpspeed/3.6 * 5s = 6.94 m
      //   trigger_dist = 5;
      // if(_gps_info->fspeed > 10)
      //   trigger_dist = 10;
      // if(_gps_info->fspeed > 15)
      //   trigger_dist = 20;

      if(gps_fix_first)
      {
        gps_last_lat = flat;
        gps_last_lon = flon;
        gps_fix_first = false;
      }
      else
      {
        // Distance in meters
        unsigned long int dist = (long int)TinyGPS::distance_between(flat, flon, gps_last_lat, gps_last_lon);

        if (dist > trigger_dist)
        {
          _gps_info->distance += dist;
          gps_last_lat = flat;
          gps_last_lon = flon;
        }
      }
    }
  } // end of single thread block
}

os_thread_return_t gpsUpdateLoop(void* param)
{
  // Start never ending loop
  for(;;) {

    while (Serial2.available())
    {
      if (_gpsReady)
        _gpsReady = false;

      char c = Serial2.read();

      if (c == '\n')  // update the info at then end of each line
        gpsInfoUpdate();

      if (_gps->encode(c) || c == '\n') // Did a new valid sentence come in?
        _gpsReady = true;
    }
  }
}

void gpsThreadSetup(TinyGPS *gps, gps_info_t *gps_info)
{
  _gps = gps;
  _gps_info = gps_info;
  
  // initialize status flags
  _gpsReady = false;

  // start the thread
  _gpsThread = new Thread("gps", gpsUpdateLoop, NULL);
}

bool gpsIsReady()
{
  return _gpsReady;
}
