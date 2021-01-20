#ifndef BIPEDAL_PID_H
#define BIPEDAL_PID_H

#include <Arduino.h>

class BipedalPID {
private:
	// PID
	double kp, ki, kd; // PID gains
	double e, previous_e, de; // Error
	double p, i, d, pid; // PID values
	double r; // Reference value or Setpoint
	double y;

	// Serial communication
	double current_time, previous_time, dt;
	char singleChar;
	short cmd_index{ 0 };
	char command[10];
public:
	// Constructor
	BipedalPID();
	BipedalPID(double kp, double ki, double kd);

	// PID
	void set_kp(double new_kp);
	void set_ki(double new_ki);
	void set_kd(double new_kd);
	void set_r(double new_r);

	// Serial communication
	void read_cmd();
	double get_val(char command[10]);
	void process_cmd();

	// PID calculation
	void update_PID();

	// Run
	void run();
};

#endif // !BIPEDAL_PID_H
