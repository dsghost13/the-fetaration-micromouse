#pragma once

#include "maze.h"
#include "queue.h"
#include "API.h"
#include <string>

// Assume this is defined somewhere else
extern Maze maze;

inline void floodfill() {
    Queue queue;

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
        queue.enqueue(goal);
        API::setText(goal.x, goal.y, std::to_string(0));
    }

    // Begin floodfill
    while (!queue.is_empty()) {
        Coord pos = queue.next();
        queue.dequeue();
        int newCost = maze.getDistance(pos) + 1;

        Cell* neighbors = maze.getNeighborCells(pos);
        for (int i = 0; i < 4; ++i) {
            Cell& neighbor = neighbors[i];
            if (!neighbor.valid || neighbor.blocked) continue;

            Coord np = neighbor.pos;
            if (newCost < maze.getDistance(np)) {
                maze.setDistance(np, newCost);
                queue.enqueue(np);
                API::setText(np.x, np.y, std::to_string(newCost));
            }
        }

        delete[] neighbors;
    }
}
