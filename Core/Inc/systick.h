#pragma once

#include "control.h"
#include "motors.h"
#include "trajectory.h"

extern MotionController motion_controller;
extern Motors motors;
extern Systick systick;

struct Systick {
	void update() {

		motion_controller.update();

		float velocity = motion_controller.get_velocity();
		float omega = motion_controller.get_omega();

		float steering_adjustment = 0.0f;
		if (motion_controller.get_motion_type() == MOTION_FORWARD && mouse.in_search_mode()) {
			steering_adjustment = get_steering_adjustment();
		}

		motors.update_controllers(velocity, omega, steering_adjustment);
	}
};
