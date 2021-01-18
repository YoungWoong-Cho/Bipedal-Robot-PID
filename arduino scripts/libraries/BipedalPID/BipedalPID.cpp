#include <Arduino.h>
#include <SoftwareSerial.h>
#include "BipedalPID.h"
#define TX 3
#define RX 2

SoftwareSerial bluetooth(TX, RX);

BipedalPID::BipedalPID() {
	Serial.begin(9600);
  bluetooth.begin(9600);
}
BipedalPID::BipedalPID(double kp, double ki, double kd) : kp(kp), ki(ki), kd(kd) {
	Serial.begin(9600);	
  bluetooth.begin(9600);
}

void BipedalPID::set_kp(double new_kp) { kp = new_kp; }
void BipedalPID::set_ki(double new_ki) { ki = new_ki; }
void BipedalPID::set_kd(double new_kd) { kd = new_kd; }
void BipedalPID::set_r(double new_r) { r = new_r; }

// Read a single command that has a format of: K2.5
void BipedalPID::read_cmd() {
	if (bluetooth.available() > 0) {
		singleChar = bluetooth.read();
		if (singleChar == '\n') {
			process_cmd();
			cmd_index = 0;
			memset(command, '\0', 10*sizeof(char));
		}
		else {
			command[cmd_index] = singleChar;
			cmd_index++;
		}
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
			Serial.print("Changing R to "); Serial.println(new_r);
			set_r(new_r);
		}
	}
	else if (command[0] == 'E') {
		if (command[1] == '\0') { Serial.print("E: "); Serial.println(e); }
	}
}

// Run
void BipedalPID::run() {
	read_cmd();
}