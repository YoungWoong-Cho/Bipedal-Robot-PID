#include <Arduino.h>
#include <SoftwareSerial.h>
#include <MPU6050.h>
#include <Wire.h>
#include "BipedalPID.h"
#define RX 2
#define TX 3
#define MOTOR_A1 4
#define MOTOR_A2 5
#define MOTOR_B1 6
#define MOTOR_B2 7
#define PWM_A 8
#define PWM_B 9

MPU6050 mpu6050(Wire);
SoftwareSerial bluetooth(TX, RX);

BipedalPID::BipedalPID() {
	Serial.begin(9600);
  bluetooth.begin(9600);
	Wire.begin();
	mpu6050.begin();

	//Setup Channel A
  pinMode(MOTOR_A1, OUTPUT); //Initiates Motor Channel A pin
  pinMode(MOTOR_A2, OUTPUT); //Initiates Motor Channel A pin

  //Setup Channel B
  pinMode(MOTOR_B1, OUTPUT); //Initiates Motor Channel B pin
  pinMode(MOTOR_B2, OUTPUT);  //Initiates Motor Channel B pin

	previous_time = millis()/1000.0; // [s]
	p = i = d = 0;
}
BipedalPID::BipedalPID(double kp, double ki, double kd) : kp(kp), ki(ki), kd(kd) {
	Serial.begin(9600);	
  bluetooth.begin(9600);
	Wire.begin();
	mpu6050.begin();

	//Setup Channel A
  pinMode(MOTOR_A1, OUTPUT); //Initiates Motor Channel A pin
  pinMode(MOTOR_A2, OUTPUT); //Initiates Motor Channel A pin

  //Setup Channel B
  pinMode(MOTOR_B1, OUTPUT); //Initiates Motor Channel B pin
  pinMode(MOTOR_B2, OUTPUT);  //Initiates Motor Channel B pin

	previous_time = millis()/1000.0; // [s]
	p = i = d = 0;
}

void BipedalPID::set_kp(double new_kp) { kp = new_kp; }
void BipedalPID::set_ki(double new_ki) { ki = new_ki; }
void BipedalPID::set_kd(double new_kd) { kd = new_kd; }
void BipedalPID::set_r(double new_r) { r = new_r; }

/* 1. Serial Communication */
// Read a single command that has a format of: K2.5
void BipedalPID::read_cmd() {
	if (bluetooth.available() > 0) {
		singleChar = bluetooth.read();
		if (singleChar == '\n') {
			process_cmd();
			update_PID();
			cmd_index = 0;
			memset(command, '\0', 10*sizeof(char));
		}
		else {
			command[cmd_index] = singleChar;
			cmd_index++;
		}
	}
	else
	{
		update_PID();
	}
}

// Return the value that comes after the keyword
double BipedalPID::get_val(char command[10]) {
	char val[9];
	for (int i = 0; i < 9; i++) {
		if (command[i] == '\0') {
			break;
		}
		else {
			val[i] = command[i + 1];
		}
	}
	return atof(val);
}

// Define what each command keyword does
void BipedalPID::process_cmd() {
	if (command[0] == 'P') {
		if (command[1] == '\0') { Serial.print("kp: "); Serial.println(kp); }
		else{
			double new_kp = get_val(command);
			Serial.print("Changing kp to "); Serial.println(new_kp);
			set_kp(new_kp);
		}
	}
	else if (command[0] == 'I') {
		if (command[1] == '\0') { Serial.print("ki: "); Serial.println(ki); }
		else{
			double new_ki = get_val(command);
			Serial.print("Changing ki to "); Serial.println(new_ki);
			set_ki(new_ki);
		}
	}
	else if (command[0] == 'D') {
		if (command[1] == '\0') { Serial.print("kd: "); Serial.println(kd); }
		else{
			double new_kd = get_val(command);
			Serial.print("Changing kd to "); Serial.println(new_kd);
			set_kd(new_kd);
		}
	}
	else if (command[0] == 'R') {
		if (command[1] == '\0') { Serial.print("R: "); Serial.println(r); }
		else{
			double new_r = get_val(command);
			//Serial.print("Changing R to "); Serial.println(new_r);
			set_r(new_r);
		}
	}
	else if (command[0] == 'E') {
		if (command[1] == '\0') { Serial.print("E: "); Serial.println(e); }
	}
}

/* 2. PID calculation */
void BipedalPID::update_PID(){
	// Time
  current_time = millis()/1000.0; // [s]
  dt = current_time - previous_time;

  // Error
	y = -mpu6050.getAngleX() + 90;
  e = r - y;
  de = e - previous_e;

  // Calculate pid values
  p = kp * e;
  i += ki * e * dt;
  d = kd * de/dt;
  pid = (p + i + d) * 255/30;

	Serial.print(" R: "); Serial.print(r); Serial.print(" Y: "); Serial.print(y); Serial.print(" E: "); Serial.print(e); Serial.print(" PID: "); Serial.println(pid);

	if (pid > 0){
    digitalWrite(MOTOR_A1, HIGH);
    digitalWrite(MOTOR_A2, LOW);
    analogWrite(PWM_A, min(pid, 255));
    digitalWrite(MOTOR_B1, HIGH);
    digitalWrite(MOTOR_B2, LOW);
    analogWrite(PWM_B, min(pid, 255));
  }
  else if (pid < 0){
    digitalWrite(MOTOR_A1, LOW);
    digitalWrite(MOTOR_A2, HIGH);
    analogWrite(PWM_A, min(-pid, 255));
    digitalWrite(MOTOR_B1, LOW);
    digitalWrite(MOTOR_B2, HIGH);
    analogWrite(PWM_B, min(-pid, 255));
  }
  previous_time = current_time;
  previous_e = e;
}
// Run
void BipedalPID::run() {
	mpu6050.update();
	read_cmd();
}