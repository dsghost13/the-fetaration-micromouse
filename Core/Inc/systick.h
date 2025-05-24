#pragma once

#include "control.h"
#include "motors.h"
#include "trajectory.h"

extern Encoders encoders;
extern MotionController motion_controller;
extern MotionProfile profile_fwd;
extern MotionProfile profile_rot;
extern Motors motors;

class Systick;
extern Systick systick;

struct Systick {
    bool start_flag = false;
    bool start_fast_mode = false;
    bool waiting_for_release = false;

    void update() {
        check_touch_trigger();

        if (start_flag) {
            reset(start_fast_mode);
            start_flag = false;
        }

        motion_controller.update();

        float velocity = motion_controller.get_velocity();
        float omega = motion_controller.get_omega();

        float steering_adjustment = 0.0f;
        if (motion_controller.get_motion_type() == MOTION_FORWARD && mouse.in_search_mode()) {
            steering_adjustment = get_steering_adjustment();
        }

        motors.update_controllers(velocity, omega, steering_adjustment);
    }

    void check_touch_trigger() {
        if (start_flag) return;

        sensors.update();

        float left_mm = sensors.wall_left_distance_mm();
        float right_mm = sensors.wall_right_distance_mm();

        if (!waiting_for_release && (left_mm < 40.0f || right_mm < 40.0f)) {
            start_fast_mode = (right_mm < 40.0f);
            waiting_for_release = true;
        }

        if (waiting_for_release && left_mm > 80.0f && right_mm > 80.0f) {
            start_flag = true;
            waiting_for_release = false;
        }
    }

    void reset(bool fast_mode) {
        encoders.reset();
        motors.stop();
        motors.reset_controllers();
        motion_controller = MotionController();
        profile_fwd = MotionProfile();
        profile_rot = MotionProfile();
        mouse.reset();

        if (fast_mode) {
            mouse.set_mode_fast();
        } else {
            mouse.set_mode_search();
        }

        floodfill();
    }
};
