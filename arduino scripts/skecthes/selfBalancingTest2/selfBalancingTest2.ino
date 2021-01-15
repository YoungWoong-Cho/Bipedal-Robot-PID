
#include <MPU6050_tockn.h>
#include <Wire.h>

static int motorA1 = 4;
static int motorA2 = 5;
static int motorB1 = 6;
static int motorB2 = 7;
static int pwmA = 10;
static int pwmB = 11;

// PID gains
float kp = 8;
float ki = 0;
float kd = 0;

// PID values
float p,d;
float i = 0;
float pid;

// Time [s]
double t;
double t_previous = 0;

float e;
float e_previous;
float r = 73.2; // Desired angle



MPU6050 mpu6050(Wire);

void setup() {
  Serial.begin(115200);
  Wire.begin();
  //Wire.setClock(50000);
  mpu6050.begin();
  mpu6050.calcGyroOffsets(true);

  //Setup Channel A
  pinMode(motorA1, OUTPUT); //Initiates Motor Channel A pin
  pinMode(motorA2, OUTPUT); //Initiates Brake Channel A pin

  //Setup Channel B
  pinMode(motorB1, OUTPUT); //Initiates Motor Channel B pin
  pinMode(motorB2, OUTPUT);  //Initiates Brake Channel B pin

  t_previous = millis()/1000.0;
  e_previous = 0;
}

void loop() {
  mpu6050.update();

  // Time
  t = millis()/1000.0; // [s]
  double dt = t - t_previous;

  // Error  
  e = r - mpu6050.getAngleX();
  float de = e - e_previous;

  // Calculate pid values
  p = kp * e;
  i += ki * e * dt;
  d = kd * de/dt;
  pid = (p + i + d) * 255/30;

  Serial.print("error: ");
  Serial.print(e);
  Serial.print("\tP control: ");
  Serial.print(p);
  Serial.print("\tI control: ");
  Serial.print(i);
  Serial.print("\tD control: ");
  Serial.println(d);
  
  if (pid > 0){
    digitalWrite(motorA1, LOW);
    digitalWrite(motorA2, HIGH);
    analogWrite(pwmA, min(pid, 255));
    digitalWrite(motorB1, LOW);
    digitalWrite(motorB2, HIGH);
    analogWrite(pwmB, min(pid, 255));
  }
  else if (pid < 0){
    digitalWrite(motorA1, HIGH);
    digitalWrite(motorA2, LOW);
    analogWrite(pwmA, min(-pid, 255));
    digitalWrite(motorB1, HIGH);
    digitalWrite(motorB2, LOW);
    analogWrite(pwmB, min(-pid, 255));
  }
  t_previous = t;
  e_previous = e;
}
