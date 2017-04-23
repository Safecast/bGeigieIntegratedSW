
SYSTEM_MODE(MANUAL);//do not connect to cloud

/*
 * This code tests the Geiger count functionality
 * and the InterruptCounter library
 */

#include "InterruptCounter.h"

void setup()
{

  Serial.begin(9600);
  
  interruptCounterSetup(D10, 5000);
  interruptCounterReset();
}

void loop()
{
  if (interruptCounterAvailable())
  {
    Serial.print("Last 5s count: ");   
    Serial.println(interruptCounterCount());
    interruptCounterReset();
  }
}

