#pragma once

#include "main.h"
#include "sensors.h"

#ifdef __cplusplus
extern "C" {
#endif

extern Sensors sensors;

// Maintain state across calls
float get_steering_adjustment() {
    static float previous_error = 0.0f;

    float left_dist = sensors.wall_left_distance_mm();
    float right_dist = sensors.wall_right_distance_mm();

    bool has_left_wall = sensors.wall_left();
    bool has_right_wall = sensors.wall_right();

    float error = 0.0f;

    if (has_left_wall && has_right_wall) {
        error = (left_dist - right_dist) / 2.0f;
    } else if (has_left_wall) {
        error = left_dist - DESIRED_WALL_DISTANCE_MM;
    } else if (has_right_wall) {
        error = DESIRED_WALL_DISTANCE_MM - right_dist;
    } else {
        return 0.0f;
    }

    float derivative = error - previous_error;
    previous_error = error;

    return STEERING_KP * error + STEERING_KD * derivative;
}

#ifdef __cplusplus
}
#endif

}

#ifdef __cplusplus
}
#endif
