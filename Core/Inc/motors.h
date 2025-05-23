#pragma once

#include "battery.h"
#include "encoders.h"
#include "main.h"

#ifdef __cplusplus

	extern Battery battery;
	extern Encoders encoders;

	class Motors {
	public:

		void disable_controllers() {
			m_controller_output_enabled = false;
		}

		void enable_controllers() {
			m_controller_output_enabled = true;
		}

		void reset_controllers() {
			m_fwd_error = 0;
			m_rot_error = 0;
			m_previous_fwd_error = 0;
			m_previous_rot_error = 0;
		}

		void stop() {
			set_left_motor_volts(0);
			set_right_motor_volts(0);
			HAL_GPIO_WritePin(ML_FWD_GPIO_Port, ML_FWD_Pin, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(ML_BWD_GPIO_Port, ML_BWD_Pin, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(MR_FWD_GPIO_Port, MR_FWD_Pin, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(MR_BWD_GPIO_Port, MR_BWD_Pin, GPIO_PIN_RESET);
		}

		float position_controller() {
			float increment = m_velocity * LOOP_INTERVAL;
			m_fwd_error += increment - encoders.robot_fwd_change();
			float diff = m_fwd_error - m_previous_fwd_error;
			m_previous_fwd_error = m_fwd_error;
			float output = FWD_KP * m_fwd_error + FWD_KD * diff;
			return output;
		}

		float angle_controller(float steering_adjustment) {
			float increment = m_omega * LOOP_INTERVAL;
			m_rot_error += increment - encoders.robot_rot_change();
			m_rot_error += steering_adjustment;
			float diff = m_rot_error - m_previous_rot_error;
			m_previous_rot_error = m_rot_error;
			float output = ROT_KP * m_rot_error + ROT_KD * diff;
			return output;
		}

		float left_feed_forward(float speed) {
			static float oldSpeed = 0;
			float leftFF = speed * SPEED_FF * BIAS_FF;
			float acc = (speed - oldSpeed) * LOOP_FREQUENCY;
			oldSpeed = speed;
			float accFF = ACC_FF * acc;
			leftFF += accFF;
			return leftFF;
		}

		float right_feed_forward(float speed) {
			static float oldSpeed = 0;
			float rightFF = speed * SPEED_FF * BIAS_FF;
			float acc = (speed - oldSpeed) * LOOP_FREQUENCY;
			oldSpeed = speed;
			float accFF = ACC_FF * acc;
			rightFF += accFF;
			return rightFF;
		}

		void update_controllers(float velocity, float omega, float steering_adjustment) {
			m_velocity = velocity;
			m_omega = omega;
			float pos_output = position_controller();
			float rot_output = angle_controller(steering_adjustment);
			float left_output = 0;
			float right_output = 0;
			left_output = pos_output - rot_output;
			right_output = pos_output + rot_output;

			float tangent_speed = m_omega * MOUSE_RADIUS * 0.01745;
			float left_speed = m_velocity - tangent_speed;
			float right_speed = m_velocity + tangent_speed;
			float left_ff = left_feed_forward(left_speed);
			float right_ff = right_feed_forward(right_speed);

			if (m_feedforward_enabled) {
				left_output += left_ff;
				right_output += right_ff;
			}

			if (m_controller_output_enabled) {
				set_right_motor_volts(right_output);
				set_left_motor_volts(left_output);
			}
		}

		int pwm_compensated(float desired_voltage, float battery_voltage) {
			int pwm = MOTOR_MAX_PWM * (desired_voltage / battery_voltage);
			return pwm;
		}

		void set_left_motor_volts(float volts) {
			int motorPWM = pwm_compensated(volts, battery.voltage());
			set_left_motor_pwm(motorPWM);
		}

		void set_right_motor_volts(float volts) {
			int motorPWM = pwm_compensated(volts, battery.voltage());
			set_right_motor_pwm(motorPWM);
		}

		void set_left_motor_pwm(int pwm) {
			if (pwm < 0) {
				HAL_GPIO_WritePin(ML_FWD_GPIO_Port, ML_FWD_Pin, GPIO_PIN_RESET);
				HAL_GPIO_WritePin(ML_BWD_GPIO_Port, ML_BWD_Pin, GPIO_PIN_SET);
				TIM2->CCR3 = fabsf(pwm);
			} else {
				HAL_GPIO_WritePin(ML_FWD_GPIO_Port, ML_FWD_Pin, GPIO_PIN_SET);
				HAL_GPIO_WritePin(ML_BWD_GPIO_Port, ML_BWD_Pin, GPIO_PIN_RESET);
				TIM2->CCR3 = fabsf(pwm);
			}
		}

		void set_right_motor_pwm(int pwm) {
			if (pwm < 0) {
				HAL_GPIO_WritePin(MR_FWD_GPIO_Port, MR_FWD_Pin, GPIO_PIN_RESET);
				HAL_GPIO_WritePin(MR_BWD_GPIO_Port, MR_BWD_Pin, GPIO_PIN_SET);
				TIM2->CCR4 = fabsf(pwm);
			} else {
				HAL_GPIO_WritePin(MR_FWD_GPIO_Port, MR_FWD_Pin, GPIO_PIN_SET);
				HAL_GPIO_WritePin(MR_BWD_GPIO_Port, MR_BWD_Pin, GPIO_PIN_RESET);
				TIM2->CCR4 = fabsf(pwm);
			}
		}

	private:
		float m_fwd_error;
		float m_rot_error;
		float m_previous_fwd_error;
		float m_previous_rot_error;

		float m_velocity;
		float m_omega;
		
		bool m_feedforward_enabled;
		bool m_controller_output_enabled;
	};

#endif
