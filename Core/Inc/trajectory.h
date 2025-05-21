#pragma once

#include "profile.h"
#include "maze.h"
#include "mouse.h"

#include <queue>
#include <stack>

extern Maze maze;
extern MotionProfile profile_fwd;
extern MotionProfile profile_rot;

class MotionController;
extern MotionController motion_controller;

enum MotionType {
    MOTION_NONE,
    MOTION_FORWARD,
    MOTION_ROTATE,
    MOTION_ARC
};

struct MotionCommand {
    float forward_distance;
    float rotate_angle;
    float top_speed;
    float final_speed;
    float acceleration;
};

class MotionController {
public:
    MotionController() : m_state(MOTION_NONE) {}

    void update() {
        if (m_state == MOTION_NONE && !m_queue.empty()) {
            start_next_command();
        }

        profile_fwd.update();
        profile_rot.update();

        if (m_state != MOTION_NONE && profile_fwd.is_finished() && profile_rot.is_finished()) {
            m_state = MOTION_NONE;
        }
    }

    void generate_search_command() {
    	if (!mouse.at_goal()) {
        	maze.update_maze();
        	Cell best_cell = maze.get_best_cell();

    		float turning_angle = get_turning_angle(best_cell);
    		if (turning_angle != 0) {
    			queue_command(MotionCommand(0, turning_angle, SEARCH_TURN_SPEED, 0, SEARCH_TURN_ACCELERATION));
    		} else {
    			queue_command(MotionCommand(FULL_CELL_MM, 0, SEARCH_FWD_SPEED, 0, SEARCH_FWD_ACCELERATION));
    			mouse.move_forward();
    		}
    	}

    }

    void generate_commands() {
    	while (!mouse.at_goal()) {
			Cell next_cell = maze.get_best_cell();

	        int turns = (cell.dir - mouse.dir + 4) % 4;
			float turning_angle = get_turning_angle(turns);
			if (turning_angle != 0) {
				queue_command(MotionCommand(0, turning_angle, FAST_TURN_SPEED, 0, FAST_TURN_ACCELERATION));
		        int turns = (cell.dir - mouse.dir + 4) % 4;
		        mouse.turn_ccw(turns);
		        next_cell = maze.get_best_cell();
			}

			float moving_distance = get_moving_distance(next_cell);
			if (moving_distance != 0) {
				queue_command(MotionCommand(moving_distance, 0, FAST_FWD_SPEED, 0, FAST_FWD_ACCELERATION));
				int count = moving_distance / FULL_CELL_MM;
				mouse.move_forward(count);
			}

			turning_angle = get_turning_angle(next_cell);
			if (turning_angle != 0) {
				float arc_length = MOUSE_RADIUS * fabsf(turning_angle);
				queue_command(MotionCommand(arc_length, turning_angle, SMOOTH_FWD_SPEED, 0, SMOOTH_FWD_ACCELERATION));
			}
		}
    }

    float get_velocity() const {
        return profile_fwd.get_current_speed();
    }

    float get_omega() const {
        return profile_rot.get_current_speed();
    }

private:
    void queue_command(const MotionCommand& cmd) {
        m_queue.push(cmd);
    }

    float get_moving_distance(Cell& cell) {
    	float distance = 0;

		while (cell.valid && cell.dir == mouse.dir) {
			distance += FULL_CELL_MM;
			if (mouse.at_goal()) return distance;
			cell = maze.get_best_cell();
		}

		return distance -= FULL_CELL_MM;
    }

    float get_turning_angle(int turns) {
    	float angle = 0;
        switch (turns) {
        	case 0: return 0;
        	case 1: return PI / 2;
        	case 2: return PI;
        	case 3: return -PI / 2;
        	default: return 0;
        }
    }

    void baacktrack() {

    }

    void start_next_command() {
		if (mouse.in_search_mode()) {
			generate_search_command();

			if (mouse.at_goal()) {
				mouse.set_mode_fast();
				backtrack();
			}
		}

		if (m_queue.empty()) return;

		MotionCommand cmd = m_queue.front();
		m_queue.pop();

		bool forward = fabsf(cmd.forward_distance) > 0.001f;
		bool rotate = fabsf(cmd.rotate_angle) > 0.001f;

		if (forward && rotate) {
			m_state = MOTION_ARC;
			profile_fwd.start(cmd.forward_distance, cmd.top_speed, cmd.final_speed, cmd.acceleration);
			float top_angular_speed = cmd.top_speed * cmd.forward_distance / cmd.rotate_angle;
			float angular_acceleration = 0; // TODO: this
			profile_rot.start(cmd.rotate_angle, top_angular_speed, cmd.final_speed, angular_acceleration);
		} else if (forward) {
			m_state = MOTION_FORWARD;
			profile_fwd.start(cmd.forward_distance, cmd.top_speed, cmd.final_speed, cmd.acceleration);
		} else if (rotate) {
			m_state = MOTION_ROTATE;
			profile_rot.start(cmd.rotate_angle, cmd.top_speed, cmd.final_speed, cmd.acceleration);
		} else {
			m_state = MOTION_NONE;
			return;
		}
	}

    MotionType m_state;
    std::queue<MotionCommand> m_queue;
    std::stack<MotionCommand> m_stack;
};
