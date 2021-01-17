#include <SoftwareSerial.h>

SoftwareSerial mySerial(3, 2); // HC-06 TX=3, RX=2

void setup()
{
  Serial.begin(9600);
  mySerial.begin(9600);
}

void loop()
{
  if (mySerial.available()){
    Serial.write(mySerial.read());
  }
  if (Serial.available()){
    mySerial.write(Serial.read());
  }
}
