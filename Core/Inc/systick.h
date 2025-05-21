#pragma once

#include "trajectory.h"
#include "motors.h"

extern MotionController motion_controller;
extern Motors motors;
extern Systick systick;

struct Systick {
	void update() {
		motion_controller.update();
		float velocity = motion_controller.get_velocity();
		float omega = motion_controller.get_omega();

		wall_follwing()

		motors.update_controllers(velocity, omega, 0.0f);
	}
};
