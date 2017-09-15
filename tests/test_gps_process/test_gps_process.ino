
SYSTEM_MODE(MANUAL);//do not connect to cloud

#include "TinyGPS.h"
#include "gps_thread.h"

TinyGPS gps(true);
#define GPS_INTERVAL 1000
gps_info_t gps_info;

// Gps data buffers
#define BUFFER_SZ 12
static char lat[BUFFER_SZ];
static char lon[BUFFER_SZ];

/* convert long integer from TinyGPS to string "xxxxx.xxxx" */
void get_coordinate_string(bool is_latitude, unsigned long val, char *buf)
{
  unsigned long left = 0;
  unsigned long right = 0;

  left = val/100000;
  right = (val - left*100000)/10;
  if (is_latitude) {
    sprintf(buf, ("%04ld.%04ld"), left, right);
  } else {
    sprintf(buf, ("%05ld.%04ld"), left, right);
  }
}

void setup()
{

  Serial.begin(57600);

  // Start the GPS Thread
  gpsThreadSetup(&gps, &gps_info);
}

void loop()
{
  Serial.printf("lat:%ld lon:%ld\n", gps_info.x, gps_info.y);
  get_coordinate_string(true, gps_info.x, lat);
  get_coordinate_string(false, gps_info.y, lon);
  Serial.printf("after processing: lat:%s lon:%s\n", lat, lon);

  Serial.printf("\n");

  delay(1000);
}
