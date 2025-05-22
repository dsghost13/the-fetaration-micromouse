#pragma once

#include "main.h"

#ifdef __cplusplus
extern "C" {
#endif

class Sensors;
extern Sensors sensors;

typedef enum {
    DIST_L,
    DIST_R,
    DIST_FL,
    DIST_FR
} dist_t;

class Sensors {
public:
    void update() {
        m_dist_L = measure_dist(DIST_L);
        m_dist_R = measure_dist(DIST_R);
        m_dist_FR = measure_dist(DIST_FR);
        m_dist_FL = measure_dist(DIST_FL);
    }

    bool wall_front() {
        return wall_front_distance_mm() < FRONT_WALL_THRESHOLD_MM;
    }

    bool wall_left() {
        return wall_left_distance_mm() < SIDE_WALL_THRESHOLD_MM;
    }

    bool wall_right() {
        return wall_right_distance_mm() < SIDE_WALL_THRESHOLD_MM;
    }

    float wall_front_distance_mm() {
        return (adc_to_mm(m_dist_FL) + adc_to_mm(m_dist_FR)) / 2.0f;
    }

    float wall_left_distance_mm() {
        return adc_to_mm(m_dist_L);
    }

    float wall_right_distance_mm() {
        return adc_to_mm(m_dist_R);
    }

private:
    uint16_t m_dist_L;
    uint16_t m_dist_R;
    uint16_t m_dist_FL;
    uint16_t m_dist_FR;

    uint16_t measure_dist(dist_t dist) {
        GPIO_TypeDef* emitter_port;
        uint16_t emitter_pin;
        GPIO_TypeDef* receiver_port;
        uint16_t receiver_pin;

        switch (dist) {
            case DIST_L:
                emitter_port    = EMIT_L_GPIO_Port;
                emitter_pin     = EMIT_L_Pin;
                receiver_port   = RECIV_L_GPIO_Port;
                receiver_pin    = RECIV_L_Pin;
                ADC1_Select_CH8();
                break;
            case DIST_R:
                emitter_port    = EMIT_R_GPIO_Port;
                emitter_pin     = EMIT_R_Pin;
                receiver_port   = RECIV_R_GPIO_Port;
                receiver_pin    = RECIV_R_Pin;
                ADC1_Select_CH5();
                break;
            case DIST_FL:
                emitter_port    = EMIT_FL_GPIO_Port;
                emitter_pin     = EMIT_FL_Pin;
                receiver_port   = RECIV_FL_GPIO_Port;
                receiver_pin    = RECIV_FL_Pin;
                ADC1_Select_CH9();
                break;
            case DIST_FR:
                emitter_port    = EMIT_FR_GPIO_Port;
                emitter_pin     = EMIT_FR_Pin;
                receiver_port   = RECIV_FR_GPIO_Port;
                receiver_pin    = RECIV_FR_Pin;
                ADC1_Select_CH4();
                break;
            default:
                return 0;
        }

        HAL_GPIO_WritePin(emitter_port, emitter_pin, GPIO_PIN_SET);
        HAL_Delay(5);

        HAL_ADC_Start(&hadc1);
        HAL_ADC_PollForConversion(&hadc1, HAL_MAX_DELAY);
        uint16_t adc_val = HAL_ADC_GetValue(&hadc1);
        HAL_ADC_Stop(&hadc1);

        HAL_GPIO_WritePin(emitter_port, emitter_pin, GPIO_PIN_RESET);

        return adc_val;
    }

    float adc_to_mm(uint16_t adc_val) {
        const float m = 0;
        const float c = 0;
        return m * adc_val + c;
    }

};

#ifdef __cplusplus
}
#endif
