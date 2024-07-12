#ifndef STATE_HPP
#define STATE_HPP

#include <cstdint>
#include <vector>

enum Node : uint8_t
{
    UNVISITED   = 0,
    WALL        = 1,
    START       = 2,
    END         = 3,
    VISITED     = 4,
    SAVED       = 5
};

struct State
{
    std::vector<Node> map;
    int width, height;
};

#endif