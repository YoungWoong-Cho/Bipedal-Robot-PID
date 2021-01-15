#include <Arduino.h>
#include "BipedalPID.h"

BipedalPID::BipedalPID() {}
BipedalPID::BipedalPID(double kp, double ki, double kd) : kp(kp), ki(ki), kd(kd) {}

void BipedalPID::set_kp(double new_kp) { kp = new_kp; }
void BipedalPID::set_ki(double new_ki) { ki = new_ki; }
void BipedalPID::set_kd(double new_kd) { kd = new_kd; }

void BipedalPID::read_cmd() {
	while (Serial.available() > 0) {
		singleChar = Serial.read();
		if (singleChar == '\n') {
			process_cmd();
			cmd_index = 0;
			memset(command, '\0', 10*sizeof(char));
			break;
		}
		else {
			command[cmd_index] = singleChar;
			cmd_index++;
		}
	}
}

double BipedalPID::get_val(char cmd[10]) {
	char val[9];
	for (int i = 0; i < 9; i++) {
		if (cmd[i] == '\0') {
			break;
		}
		else {
			val[i] = cmd[i + 1];
		}
	}
	return atof(val);
}

void BipedalPID::process_cmd() {
	if (command[0] == 'P') {
		double new_kp = get_val(command);
		Serial.print("Changing kp into "); Serial.println(new_kp);
		set_kp(new_kp);
	}
	else if (command[0] == 'I') {
		double new_ki = get_val(command);
		Serial.print("Changing ki into "); Serial.println(new_ki);
		set_ki(new_ki);
	}
	else if (command[0] == 'D') {
		double new_kd = get_val(command);
		Serial.print("Changing kd into "); Serial.println(new_kd);
		set_kd(new_kd);
	}
}

void BipedalPID::run() {
	read_cmd();
}