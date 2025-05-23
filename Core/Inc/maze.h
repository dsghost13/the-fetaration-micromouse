#pragma once

#include "coord.h"
#include "main.h"
#include "mouse.h"
#include "sensors.h"

#include <string>
#include <cmath>
#include <cstdint>

class Maze;
extern Maze maze;

extern Mouse mouse;

#define MAZE_SIZE 16
#define MAX_COST 255

struct Cell {
    Coord pos;
    Direction dir;
    bool blocked;
    bool valid;

    Cell() : pos(), dir(NORTH), blocked(false), valid(false) {}
    Cell(Coord p, Direction d, bool wall) : pos(p), dir(d), blocked(wall), valid(true) {}
};

class Maze {
public:
	Coord* goalPos;

    bool at_goal() const {
        for (int i = 0; i < 4; ++i) {
            if (goalPos[i] == mouse.pos) {
            	return true;
            }
        }
        return false;
    }

	void set_goal_cells(bool backtracking = false) {
		if (backtracking) {
			goalPos = new Coord[1];
			goalPos[0] = Coord(0, 0);
		} else {
			goalPos = new Coord[4];
			goalPos[0] = Coord(7, 7);
			goalPos[1] = Coord(7, 8);
			goalPos[2] = Coord(8, 7);
			goalPos[3] = Coord(8, 8);
		}
	}

    Maze() {
        set_goal_cells();
    }

    ~Maze() {
        delete[] goalPos;
    }

    Cell* get_neighbor_cells(Coord pos) {
        Cell* neighborCells = new Cell[4];
        for (int i = 0; i < 4; ++i) {
            Coord neighborPos = getNeighborPos(pos, i);
            if (isValidPos(neighborPos)) {
                bool blocked = (cellWalls[pos.x][pos.y] & (1 << i)) != 0;
                neighborCells[i] = Cell(neighborPos, Direction(i), blocked);
            }
        }
        return neighborCells;
    }

    Cell get_best_cell() {
        Cell* neighborCells = get_neighbor_cells(mouse.pos);
        Cell bestCell;

        for (int i = 0; i < 4; ++i) {
            if (neighborCells[i].valid && !neighborCells[i].blocked) {
                if (!bestCell.valid) {
                    bestCell = neighborCells[i];
                } else {
                    int currDist = distances[neighborCells[i].pos.x][neighborCells[i].pos.y];
                    int bestDist = distances[bestCell.pos.x][bestCell.pos.y];
                    if (currDist < bestDist) {
                        bestCell = neighborCells[i];
                    }
                }
            }
        }

        delete[] neighborCells;
        return bestCell;
    }

    void rotateCCW(int turns = 1) {
        mouse.dir = Direction((mouse.dir + turns) % 4);
    }

    void moveForward() {
        mouse.pos.x += NEIGHBOR_DISTANCES[mouse.dir][0];
        mouse.pos.y += NEIGHBOR_DISTANCES[mouse.dir][1];
    }

    void set_wall(int offset) {
        int wallDir = (mouse.dir + offset) % 4;
        int oppDir = (wallDir + 2) % 4;
        Coord neighborPos = getNeighborPos(mouse.pos, wallDir);

        cellWalls[mouse.pos.x][mouse.pos.y] |= (1 << wallDir);

        if (isValidPos(neighborPos)) {
            cellWalls[neighborPos.x][neighborPos.y] |= (1 << oppDir);
        }
    }

    void update_walls() {
        if (sensors.wall_front()) set_wall(0);
        if (sensors.wall_left()) set_wall(1);
        if (sensors.wall_right()) set_wall(3);
    }

    Coord* getGoalCells() const {
        return goalPos;
    }

    int getDistance(Coord pos) const {
        return distances[pos.x][pos.y];
    }

    void setDistance(Coord pos, int cost) {
        distances[pos.x][pos.y] = cost;
    }

    bool isValidPos(Coord pos) const {
        return pos.x >= 0 && pos.x < MAZE_SIZE && pos.y >= 0 && pos.y < MAZE_SIZE;
    }

    Coord getNeighborPos(Coord pos, int dir) {
        int dx = NEIGHBOR_DISTANCES[dir][0];
        int dy = NEIGHBOR_DISTANCES[dir][1];
        return Coord(pos.x + dx, pos.y + dy);
    }

    Cell** getCellWalls() {
        return reinterpret_cast<Cell**>(cellWalls);
    }

    uint8_t cellWalls[MAZE_SIZE][MAZE_SIZE];
    int distances[MAZE_SIZE][MAZE_SIZE];

private:
    char dirToChar(int dir) {
        switch (dir) {
            case 0: return 'n';
            case 1: return 'w';
            case 2: return 's';
            case 3: return 'e';
            default: return '\0';
        }
    }
};

