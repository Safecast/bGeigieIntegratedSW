
SYSTEM_MODE(MANUAL);//do not connect to cloud

#include "NanoConfig.h"

void setup()
{
  Serial.begin(9600);

  pinMode(D7, OUTPUT);
  digitalWrite(D7, HIGH);

  pinMode(PWR_ON, OUTPUT);
  digitalWrite(PWR_ON, HIGH); // keep power one

  pinMode(PWR_OFF, INPUT);  // sense power switch state
}

void loop()
{
  // Perform check for Soft shutdown
  int power_off_status = digitalRead(PWR_OFF); // pin read high when switch is off
  if (power_off_status == HIGH)
  {
    Serial.println("Shutdown now");
    delay(1000);
    digitalWrite(PWR_ON, LOW);
    while(1)
      ;
  }

  Serial.println("I'm running.");
  digitalWrite(D7, HIGH);
  delay(500);
  digitalWrite(D7, LOW);
  delay(500);
}
