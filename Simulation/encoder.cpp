#ifndef ENCODER_H
#define ENCODER_H

#define TICKS_PER_ROTATION 12
#define GEAR_RATIO 30      
#define TICKS_PER_WHEEL_REV  (TICKS_PER_ROTATION * GEAR_RATIO)
 
#define WHEEL_CIRCUMFERENCE_MM 32 * 3.14159

#define MPER_COUNT_LEFT (WHEEL_CIRCUMFERENCE_MM / TICKS_PER_WHEEL_REV)  
#define MPER_COUNT_RIGHT (WHEEL_CIRCUMFERENCE_MM / TICKS_PER_WHEEL_REV)

#define DEG_PER_MDIFFERENCE 1  // TODO: calculate this

#define LOOP_FREQUENCY 1000  // TODO: find this

/*
    // in main.c

    uint16_t enc_left = 0;
    uint16_t enc_right = 0;

    void HAL_TIIC_CaptureCallback(TIHandleTypeDef *htim) {
        if (htim->Instance == TIM3) {  
            enc_left = __HAL_TIGET_COUNTER(htim);
        }
        else if (htim->Instance == TIM4) { 
            enc_right = __HAL_TIGET_COUNTER(htim);
        }
    }
*/
 
struct Encoders {

    float fwd_change;
    float rot_change;
    int left_counter;
    int right_counter;
    float robot_distance;
    float robot_angle;

    Encoders() {
        reset();
    }

    void begin() {
        reset();
    }

    void reset() {
        left_counter = 0;
        right_counter = 0;
        robot_distance = 0;
        robot_angle = 0;
    }

    void update() {
        int left_delta = 0;
        int right_delta = 0;

        left_delta = left_counter;
        right_delta = right_counter;
        left_counter = 0;
        right_counter = 0;

        float left_change = left_delta * MPER_COUNT_LEFT;
        float right_change = right_delta * MPER_COUNT_RIGHT;

        fwd_change = 0.5 * (right_change + left_change);
        robot_distance += fwd_change;

        rot_change = (right_change - left_change) * DEG_PER_MDIFFERENCE;
        robot_angle += rot_change;
    }

    float get_robot_distance() {
        return robot_distance;
    }

    float get_robot_speed() {
        float speed = LOOP_FREQUENCY * fwd_change;
        return speed;
    }

    float get_robot_omega() {
        float omega = LOOP_FREQUENCY * rot_change;
        return omega;
    }

    float get_robot_angle() {
        return robot_angle;
    }
};

#endif /* ENCODER_H */ 