#ifndef __GPS_THREAD__
#define __GPS_THREAD__

#include "TinyGPS.h"

void gpsThreadSetup(TinyGPS *gps);
bool gpsIsReady();

#endif
