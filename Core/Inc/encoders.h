#pragma once

#include "main.h"

#ifdef __cplusplus
extern "C" {
#endif

	extern TIM_HandleTypeDef htim3;
	extern TIM_HandleTypeDef htim4;

	class Encoders;
	extern Encoders encoders;

	class Encoders {
	public:
		Encoders() : m_enc_left(0), m_enc_right(0), m_prev_enc_left(0), m_prev_enc_right(0), m_left_distance(0.0), m_right_distance(0.0) {}

		uint32_t left_count = 0;
		uint32_t right_count = 0;

		void update(TIM_HandleTypeDef* htim) {
		    if (htim->Instance == TIM3) {
		        left_count = __HAL_TIM_GET_COUNTER(htim);
		    } else if (htim->Instance == TIM4) {
		        right_count = __HAL_TIM_GET_COUNTER(htim);
		    }
		}

		void initialize() {
			HAL_TIM_Encoder_Start(&htim3, TIM_CHANNEL_ALL);
			HAL_TIM_Encoder_Start(&htim4, TIM_CHANNEL_ALL);
			reset();
		}

		void reset() {
			__HAL_TIM_SET_COUNTER(&htim3, 0);
			__HAL_TIM_SET_COUNTER(&htim4, 0);

			m_enc_left = 0;
			m_enc_right = 0;
			m_prev_enc_left = 0;
			m_prev_enc_right = 0;
			m_left_distance = 0;
			m_right_distance = 0;
		}

		void update_robot_distance() {
			int16_t delta_enc_left = m_enc_left - m_prev_enc_left;
			int16_t delta_enc_right = m_enc_right - m_prev_enc_right;

			m_left_distance = (delta_enc_left / LOOP_INTERVAL) * (PI *  WHEEL_DIAMETER);
			m_right_distance = (delta_enc_right / LOOP_INTERVAL) * (PI * WHEEL_DIAMETER);

			m_prev_enc_left = m_enc_left;
			m_prev_enc_right = m_enc_right;
		}

		float robot_fwd_change() {
			update_robot_distance();
			return (m_left_distance + m_right_distance) / 2.0;
		}

		float robot_rot_change() {
			update_robot_distance();
			return (m_left_distance - m_right_distance) / TRACK_WIDTH;
		}

		void update_encoder_counts() {
			m_enc_left = (int16_t)__HAL_TIM_GET_COUNTER(&htim3);
			m_enc_right = (int16_t)__HAL_TIM_GET_COUNTER(&htim4);
		}

		int16_t get_left_encoder_count() {
			return m_enc_left;
		}

		int16_t get_right_encoder_count() {
			return m_enc_right;
		}

	private:
		int16_t m_enc_left;
		int16_t m_enc_right;
		int16_t m_prev_enc_left;
		int16_t m_prev_enc_right;

		float m_left_distance;
		float m_right_distance;
	};



#ifdef __cplusplus
}
#endif
