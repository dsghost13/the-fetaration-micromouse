#pragma once

#include "coord.h"
#include "maze.h"

#include <queue>
#include <string>

// Assume this is defined somewhere else
extern Maze maze;

inline void floodfill() {
    std::queue<Coord> queue;

    // Initialize all distances to MAX_COST
    for (int x = 0; x < MAZE_SIZE; ++x) {
        for (int y = 0; y < MAZE_SIZE; ++y) {
            maze.distances[x][y] = MAX_COST;
        }
    }

    // Push all goal cells into the queue and set them to 0
    for (int i = 0; i < 4; ++i) {
        Coord goal = maze.getGoalCells()[i];
        maze.setDistance(goal, 0);
        queue.push(goal);
    }

    // Begin floodfill
    while (!queue.empty()) {
        Coord pos = queue.front();
        queue.pop();
        int newCost = maze.getDistance(pos) + 1;

        Cell* neighbors = maze.get_neighbor_cells(pos);
        for (int i = 0; i < 4; ++i) {
            Cell& neighbor = neighbors[i];
            if (!neighbor.valid || neighbor.blocked) continue;

            Coord np = neighbor.pos;
            if (newCost < maze.getDistance(np)) {
                maze.setDistance(np, newCost);
                queue.push(np);
            }
        }

        delete[] neighbors;
    }
}
