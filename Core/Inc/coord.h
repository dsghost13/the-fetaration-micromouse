#ifndef INC_COORD_H_
#define INC_COORD_H_

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

#endif /* INC_COORD_H_ */
