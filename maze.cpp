/*
* maze.cpp
*
*  Created on: Mar 2, 2025
*      Author: Peter Tan Dat Le
*/

#include "API.h"
#include "maze.h"

Maze::Maze() : mousePos(Coord(0,0)), mouseDir(NORTH) {
    setGoalCells();
    floodfill(); // sets starter Manhattan distances
}

Cell* Maze::getNeighborCells(Coord pos) {
    Cell* neighborCells = new Cell[4]; // one possible cell per cardinal direction 

    // checks through all four directions
    for (int i = 0; i < 4; ++i) {
        Coord neighborPos = getNeighborPos(pos, i);

        // adds neigbor cell if it exists
        if (isValidPos(neighborPos)) {
            bool blocked = (cellWalls[pos.x][pos.y] & (1 << i)) != 0;
            Cell newCell = Cell(Coord(neighborPos.x, neighborPos.y), Direction(i), blocked);
            neighborCells[i] = newCell;
        }
    }

    return neighborCells;
}

Cell Maze::getBestCell() {
    Cell* neighborCells = getNeighborCells(mousePos);
    Cell bestCell;

    for (int i = 0; i < 4; ++i) {
        // only wants reachable cells
        if (neighborCells[i].valid && !neighborCells[i].blocked) {
            if (!bestCell.valid) { // case for no best cell defined yet
                bestCell = neighborCells[i];
            } else {
                int currentDistance = distances[neighborCells[i].pos.x][neighborCells[i].pos.y];
                int bestDistance = distances[bestCell.pos.x][bestCell.pos.y];
                
                // saves if the current distance is the shortest so far
                if (currentDistance < bestDistance) {
                    bestCell = neighborCells[i];
                }
            }
        }
    }

    delete[] neighborCells;
    return bestCell;
}

void Maze::rotateCCW(int turns) {
    mouseDir = Direction((mouseDir + turns) % 4);
}

// moves mouse forward one cell in the current direction
void Maze::moveForward() {
    mousePos.x += NEIGHBOR_DISTANCES[mouseDir][0];
    mousePos.y += NEIGHBOR_DISTANCES[mouseDir][1];
}

void Maze::setGoalCells() {
    goalPos = new Coord[4];

    // standard Micromouse goal cells
    goalPos[0] = Coord(7,7);
    goalPos[1] = Coord(7,8);
    goalPos[2] = Coord(8,7);
    goalPos[3] = Coord(8,8);
}

void Maze::floodfill() {
    // initializes all cell distances with the max cost
    for (int i = 0; i < 16; ++i) {
        for (int j = 0; j < 16; ++j) {
            distances[i][j] = MAX_COST;
        }
    }

    // adds all goal cells to queue and sets their distances to 0
    for (int i = 0; i < 4; ++i) {
        distances[goalPos[i].x][goalPos[i].y] = 0;
        queue.enqueue(goalPos[i]);
        API::setText(goalPos[i].x, goalPos[i].y, std::to_string(0));
    }

    // propagates entire maze with Manhattan distances
    while (!queue.is_empty()) {
        // gets next cell for incremental propogation
        Coord pos = queue.next();
        queue.dequeue();
        int newCost = distances[pos.x][pos.y] + 1;

        // propagates all reachable neighboring cells
        Cell* neighborCells = getNeighborCells(pos);
        for(int i = 0; i < 4; ++i) {
            // checks for valid, unblocked cells
            if (neighborCells[i].valid && !neighborCells[i].blocked) {
                // replaces current cost if new cost is lower 
                if(newCost < distances[neighborCells[i].pos.x][neighborCells[i].pos.y]) {
                    distances[neighborCells[i].pos.x][neighborCells[i].pos.y] = newCost;
                    queue.enqueue(neighborCells[i].pos); // adds neighboring cells for propagation on future loops
                    API::setText(neighborCells[i].pos.x, neighborCells[i].pos.y, std::to_string(newCost));
                }
            }
        }

        delete[] neighborCells;
    }
}

// gets coordinates of neighbor in given direction
Coord Maze::getNeighborPos(Coord pos, int dir) {
    int neighborX = pos.x + NEIGHBOR_DISTANCES[dir][0];
    int neighborY = pos.y + NEIGHBOR_DISTANCES[dir][1];

    Coord neighborPos = Coord(neighborX, neighborY);
    return neighborPos;
}

// checks if coordinate is within the 16x16 maze
bool Maze::isValidPos(Coord pos) {
    bool xOutOfBounds = (pos.x < 0) || (pos.x >= 16);
    bool yOutOfBounds = (pos.y < 0) || (pos.y >= 16);
    return (!xOutOfBounds && !yOutOfBounds);
}

// mackerone's mms uses characters instead of integers to represent directions
char dirToChar(int dir) {
    switch (dir) {
        case 0: 
            return 'n';
        case 1:
            return 'w';
        case 2:
            return 's';
        case 3:
            return 'e';
        default:
            return '\0';
    }
}

// sets wall in desired direction given offset from current mouse direction
void Maze::setWall(int offset) {
    int wallDir = (mouseDir + offset) % 4;
    int oppDir = (wallDir + 2) % 4;
    Coord neighborPos = getNeighborPos(mousePos, wallDir);

    cellWalls[mousePos.x][mousePos.y] |= (1 << wallDir);
    API::setWall(mousePos.x, mousePos.y, dirToChar(wallDir));

    // adjecent cells can share the same wall
    if (isValidPos(neighborPos)) 
        cellWalls[neighborPos.x][neighborPos.y] |= (1 << oppDir); 
}

// updates walls for current cell and recalculates Manhattan distances
void Maze::updateWalls() {
    if (API::wallFront()) 
        setWall(0);
    if (API::wallLeft()) 
        setWall(1);
    if (API::wallRight()) 
        setWall(3);
    floodfill();
}

// moves simulator through one cell traversal 
void Maze::updateSimulator() {
    updateWalls();
    Cell bestCell = getBestCell(); 
    int turnsNeeded = (bestCell.dir - mouseDir + 4) % 4;

    switch (turnsNeeded) {
        case 1: 
            rotateCCW();
            API::turnLeft();
            break;
        case 2:
            // turns around for dead ends
            rotateCCW(2);
            API::turnRight();
            API::turnRight();
            floodfill(); //re-evaluates Manhattan distances when stuck
            break;
        case 3:
            rotateCCW(3);
            API::turnRight();
            break;
        default:
            break;
    }

    moveForward();
    API::moveForward();
}

Maze::~Maze() {
    delete[] goalPos;
} 