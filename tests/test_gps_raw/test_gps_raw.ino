
SYSTEM_MODE(MANUAL);//do not connect to cloud

void setup()
{
  Serial.begin(57600);
  Serial2.begin(9600);
}

void loop()
{
  while (Serial2.available())
  {
    Serial.write(Serial2.read());
  }
}
