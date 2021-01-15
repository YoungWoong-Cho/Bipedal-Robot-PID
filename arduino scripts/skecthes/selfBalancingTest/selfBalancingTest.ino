
#include <MPU6050_tockn.h>
#include <Wire.h>

static int motorA = 12;
static int motorB = 13;
static int brakeA = 9;
static int brakeB = 8;
static int pwmA = 3;
static int pwmB = 11;

MPU6050 mpu6050(Wire);

void setup() {
  Serial.begin(1000000);
  Wire.begin();
  Wire.setClock(50000);
  mpu6050.begin();
  mpu6050.calcGyroOffsets(true);

  //Setup Channel A
  pinMode(motorA, OUTPUT); //Initiates Motor Channel A pin
  pinMode(brakeA, OUTPUT); //Initiates Brake Channel A pin

  //Setup Channel B
  pinMode(motorB, OUTPUT); //Initiates Motor Channel B pin
  pinMode(brakeB, OUTPUT);  //Initiates Brake Channel B pin

  // Disengage the Brake for Channel A and B
  digitalWrite(brakeA, LOW);   //Disengage the Brake for Channel A
  digitalWrite(brakeB, LOW);   //Disengage the Brake for Channel A
}

void loop() {
  mpu6050.update();
  Serial.print("angleX : ");
  Serial.println(mpu6050.getAngleX());

  if (mpu6050.getAngleX() < 90)
  {
    // Actuate motor A
    digitalWrite(motorA, HIGH); //Establishes forward direction of Channel A
    analogWrite(pwmA, 255);
  
    // Actuate motor B
    digitalWrite(motorB, HIGH); //Establishes forward direction of Channel A
    analogWrite(pwmB, 255); 
  }
  else if (mpu6050.getAngleX() > 90)
  {
    // Actuate motor A
    digitalWrite(motorA, LOW); //Establishes forward direction of Channel A
    analogWrite(pwmA, 255);
  
    // Actuate motor B
    digitalWrite(motorB, LOW); //Establishes forward direction of Channel A
    analogWrite(pwmB, 255); 
  }
}
