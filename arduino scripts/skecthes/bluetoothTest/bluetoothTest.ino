#include <SoftwareSerial.h>

SoftwareSerial mySerial(11, 10); // HC-06 TX=11, RX=10

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
