/*
* maze.h
*
*  Created on: Mar 2, 2025
*      Author: Peter Tan Dat Le
*/

#ifndef MAZE_H
#define MAZE_H

#include "queue.h"

struct Coord {
    int x;
    int y;

    Coord() : x(-1), y(-1) {}; // default for array declaration
    Coord(int x, int y) : x(x), y(y) {};
};

enum Direction {
    NORTH = 0,
    WEST = 1,
    SOUTH = 2,
    EAST = 3
};

struct Cell {
    Coord pos;
    Direction dir;
    bool blocked;
    bool valid; 

    Cell() : pos(Coord(-1,-1)), dir(NORTH), blocked(true), valid(false) {}; // default for array declaration
    Cell(Coord pos, Direction dir, bool blocked) : pos(pos), dir(dir), blocked(blocked), valid(true) {};
};

struct Maze {
    Coord mousePos;
    Direction mouseDir;
    int distances[16][16];
    int cellWalls[16][16] = {};
    Coord* goalPos;
    Queue<Coord> queue;

    const int MAX_COST = 255;
    const int NEIGHBOR_DISTANCES[4][2] = {{0,1}, {-1,0}, {0,-1}, {1,0}};

    Maze();
    Cell* getNeighborCells(Coord pos);
    Cell getBestCell();
    void setGoalCells();
    void floodfill();
    void rotateCCW(int turns = 1);
    void moveForward(); 
    bool isValidPos(Coord pos);
    Coord getNeighborPos(Coord pos, int dir);
    void setWall(int offset);
    void updateWalls();
    void updateSimulator();
    ~Maze();
};

#endif /* MAZE_H */ 