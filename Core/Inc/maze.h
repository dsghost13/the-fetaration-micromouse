#pragma once

#include "API.h"
#include "main.h"
#include "mouse.h"

#include <string>
#include <cmath>
#include <cstdint>

extern Mouse mouse;

enum Direction {
    NORTH = 0,
    WEST  = 1,
    SOUTH = 2,
    EAST  = 3
};

const int NEIGHBOR_DISTANCES[4][2] = {
    {0, 1},
    {-1, 0},
    {0, -1},
    {1, 0}
};

struct Coord {
    int x;
    int y;

    Coord(int x = 0, int y = 0) : x(x), y(y) {}

    bool operator==(const Coord& other) const {
        return x == other.x && y == other.y;
    }
};

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
    Maze() {
        setGoalCells();
    }

    ~Maze() {
        delete[] goalPos;
    }

    Cell* getNeighborCells(Coord pos) {
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
        Cell* neighborCells = getNeighborCells(mouse.pos);
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

    void setWall(int offset) {
        int wallDir = (mouse.dir + offset) % 4;
        int oppDir = (wallDir + 2) % 4;
        Coord neighborPos = getNeighborPos(mouse.pos, wallDir);

        cellWalls[mouse.pos.x][mouse.pos.y] |= (1 << wallDir);
        API::setWall(mouse.pos.x, mouse.pos.y, dirToChar(wallDir));

        if (isValidPos(neighborPos)) {
            cellWalls[neighborPos.x][neighborPos.y] |= (1 << oppDir);
        }
    }

    void updateWalls() {
        if (API::wallFront()) setWall(0);
        if (API::wallLeft()) setWall(1);
        if (API::wallRight()) setWall(3);
    }

    void updateSimulator() {
        updateWalls();
        Cell bestCell = get_best_cell();
        int turnsNeeded = (bestCell.dir - mouse.dir + 4) % 4;

        switch (turnsNeeded) {
            case 1: rotateCCW(); API::turnLeft(); break;
            case 2: rotateCCW(2); API::turnRight(); API::turnRight(); break;
            case 3: rotateCCW(3); API::turnRight(); break;
        }

        moveForward();
        API::moveForward();
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
    Coord* goalPos;

    void setGoalCells() {
        goalPos = new Coord[4];
        goalPos[0] = Coord(7, 7);
        goalPos[1] = Coord(7, 8);
        goalPos[2] = Coord(8, 7);
        goalPos[3] = Coord(8, 8);
    }

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

extern Mouse mouse;

