#pragma once

#include "maze.h"

extern Mouse mouse;

struct Mouse {
    Coord pos;
    Direction dir;
    bool search_mode;

    Mouse() : pos(0, 0), dir(NORTH), search_mode(true) {}

    void set_mode_search() {
    	search_mode = true;
    }

    void set_mode_fast() {
    	search_mode = false;
    }

    void move_forward() {
		pos.x += NEIGHBOR_DISTANCES[dir][0];
		pos.y += NEIGHBOR_DISTANCES[dir][1];
    }

    void turn_ccw(int turns = 1) {
        dir = Direction((dir + turns) % 4);
    }

    bool in_search_mode() const {
    	return search_mode;
    }

    bool at_goal() const {
        for (int i = 0; i < 4; ++i) {
            if (goalPos[i] == pos) {
            	return true;
            }
        }
        return false;
    }
};
