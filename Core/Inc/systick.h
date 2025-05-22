#pragma once

#include "trajectory.h"
#include "motors.h"
#include <cmath>


extern MotionController motion_controller;
extern Motors motors;
extern Systick systick;

struct Systick {
	float get_steering_adjustment() { //PLUS SOMEHOW ADD A BOOL INPLACE
		if(wall_left && wall_right && entered_square){ //IF IT ENTERS A SQUARE AND THERES WALLS ON THE SIDES...
			if ( L<R && fasbf(L-R) > 10){ //IF THE DISTANCE FROM L TO R
				float length = sqrt(180*180 + (L-R)*(L-R)/4);
				float angle = asin(180/l);
				INPLACE = false;
				oldAngle = angle;
				return angle;
			}
			if else( L>R && fasbf(L-R) > 10){
				float length = sqrt(180*180 + (L-R)*(L-R)/4);
				float angle = -asin(180/l);
				INPLACE = false;
				oldAngle = angle;
				return angle;
			}	
			if else(oldAngle<0){
				INPLACE = true;
				olderAngle = oldAngle;
				oldAngle = 0;
				return -olderAngle;
			}
		}
		else{
			INPLACE = false;
			return 0.0;
		}	
	}
	
	void update() {
		motion_controller.update();
		float velocity = motion_controller.get_velocity();
		float omega = motion_controller.get_omega();

		wall_following();

		motors.update_controllers(velocity, omega, 0.0f);
		
	
	}
};
