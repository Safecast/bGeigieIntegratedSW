#ifndef __GPS_THREAD__
#define __GPS_THREAD__

#include "TinyGPS.h"

#define GPS_AVAILABLE 'A'  // indicates gps data are ready (available)
#define GPS_VOID      'V'  // indicates gps data not ready (void)
#define DEFAULT_YEAR 2013

typedef struct
{
  int year = DEFAULT_YEAR;
  byte month = 0, day = 0, hour = 0, minute = 0, second = 0, hundredths = 0;
  long int x = 0, y = 0;
  float faltitude = 0, fspeed = 0;
  unsigned short nbsat = 0;
  unsigned long precision = 0;
  unsigned long age;
  char NS = 'N';
  char WE = 'E';
  char gps_status = GPS_VOID;
  unsigned long int distance = 0;
}
gps_info_t;

void gpsThreadSetup(TinyGPS *gps, gps_info_t *gps_info);
bool gpsIsReady();

#endif
