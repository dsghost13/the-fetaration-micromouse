#pragma once

#include "coord.h"
#include "maze.h"

class Mouse;
extern Mouse mouse;

struct Mouse {
    Coord pos;
    Direction dir;
    bool search_mode;

    Mouse() : pos(0, 0), dir(NORTH), search_mode(true) {}

    void reset() {
    	pos.x = 0;
    	pos.y = 0;
    	dir = NORTH;
    }

    void set_mode_search() {
    	search_mode = true;
    }

    void set_mode_fast() {
    	search_mode = false;
    }

    void move_forward(int count = 1) {
		pos.x += NEIGHBOR_DISTANCES[dir][0] * count;
		pos.y += NEIGHBOR_DISTANCES[dir][1] * count;
    }

    void turn_ccw(int turns = 1) {
        dir = Direction((dir + turns) % 4);
    }

    bool in_search_mode() const {
    	return search_mode;
    }
};
