#pragma once

#include "main.h"
#include "sensors.h"

#ifdef __cplusplus
extern "C" {
#endif

extern Sensors sensors;

float get_steering_adjustment() {
    float left_dist = sensors.wall_left_distance_mm();
    float right_dist = sensors.wall_right_distance_mm();

    bool has_left_wall = sensors.wall_left();
    bool has_right_wall = sensors.wall_right();

    float error = 0.0f;

    if (has_left_wall && has_right_wall) {
        error = (left_dist - right_dist) / 2;
    } else if (has_left_wall) {
        error = left_dist - DESIRED_WALL_DISTANCE_MM;
    } else if (has_right_wall) {
        error = DESIRED_WALL_DISTANCE_MM - right_dist;
    } else {
        return 0.0f;
    }

    return STEERING_KP * error;
}

#ifdef __cplusplus
}
#endif
