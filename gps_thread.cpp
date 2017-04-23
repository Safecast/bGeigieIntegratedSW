
#include "gps_thread.h"

Thread *_gpsThread;

TinyGPS *_gps;

bool _gpsReady = false;

os_thread_return_t gpsUpdateLoop(void* param){
    
    // Start never ending loop
    for(;;) {

      while (Serial2.available())
      {
        if (_gpsReady)
          _gpsReady = false;

        char c = Serial2.read();

        if (_gps->encode(c)) // Did a new valid sentence come in?
          _gpsReady = true;
      }
    }
}

void gpsThreadSetup(TinyGPS *gps)
{
  _gps = gps;
  
  // initialize status flags
  _gpsReady = false;

  // start the thread
  _gpsThread = new Thread("gps", gpsUpdateLoop, NULL);
}

bool gpsIsReady()
{
  return _gpsReady;
}
