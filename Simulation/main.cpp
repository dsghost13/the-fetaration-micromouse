#include <iostream>
#include <string>

#include "API.h"
#include "maze.h"

void log(const std::string& text) {
    std::cerr << text << std::endl;
}

int main(int argc, char* argv[]) {
    log("Running...");
    API::setColor(0, 0, 'G');
    API::setText(0, 0, "abc");

    Maze maze = Maze();
    while (maze.distances[maze.mousePos.x][maze.mousePos.y] != 0) {
        maze.updateSimulator();
    }
}