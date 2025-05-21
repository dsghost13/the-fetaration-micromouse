#pragma once

#include "main.h"

#ifdef __cplusplus
extern "C" {
#endif

	class MotionProfile;
	extern MotionProfile profile;

	enum ProfileState {
		IDLE,
		ACCEL,
		CONST,
		BRAKE,
		FINISHED
	};

	class MotionProfile {
	public:
		MotionProfile() {
			m_state = IDLE;
			m_distance = 0;
			m_top_speed = 0;
			m_final_speed = 0;
			m_acceleration = 0;
			m_current_speed = 0;
			m_current_position = 0;
		}

		void start(float distance, float top_speed, float final_speed, float acceleration) {
			m_distance = distance;
			m_top_speed = top_speed;
			m_final_speed = final_speed;
			m_acceleration = acceleration;

			m_current_position = 0;
			m_current_speed = 0;

			m_state = ACCEL;
		}

		void update() {
			if (m_state == FINISHED) return;

			float dt = 0.001f;
			float delta_v = m_acceleration * dt;

			switch (m_state) {
				case ACCEL:
					if (get_braking_distance(m_current_speed, m_final_speed, m_acceleration) >= m_distance - m_current_position) {
						m_state = BRAKE;
						break;
					}
					m_current_speed += delta_v;
					if (m_current_speed >= m_top_speed) {
						m_current_speed = m_top_speed;
						m_state = CONST;
					}
					break;

				case CONST:
					if (get_braking_distance(m_current_speed, m_final_speed, m_acceleration) >= m_distance - m_current_position) {
						m_state = BRAKE;
					}
					break;

				case BRAKE:
					if (m_current_speed > m_final_speed) {
						m_current_speed -= delta_v;
						if (m_current_speed < m_final_speed) {
							m_current_speed = m_final_speed;
						}
					} else {
						m_current_speed += delta_v;
						if (m_current_speed > m_final_speed) {
							m_current_speed = m_final_speed;
						}
					}
					break;

				default:
					break;
			}

			m_current_position += m_current_speed * dt;

			if ((m_distance - m_current_position) <= 0.001f || m_current_position >= m_distance) {
				m_current_position = m_distance;
				m_current_speed = m_final_speed;
				m_state = FINISHED;
			}
		}

		float get_current_speed() {
			return m_current_speed;
		}

		float get_current_position() {
			return m_current_position;
		}

		bool is_finished() {
			return m_state == FINISHED;
		}

	private:
		ProfileState m_state;

		float m_distance;
		float m_top_speed;
		float m_final_speed;
		float m_acceleration;

		float m_current_speed;
		float m_current_position;

		float get_braking_distance(float current_speed, float final_speed, float acceleration) {
			return (current_speed * current_speed - final_speed * final_speed) / (2.0f * acceleration);
		}
	};

#ifdef __cplusplus
}
#endif
