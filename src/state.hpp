#ifndef STATE_HPP
#define STATE_HPP

#include <cstdint>
#include <vector>
#include <string>

struct Point
{
    int x;
    int y;

    bool operator==(const Point& p) const = default;
    bool operator!=(const Point& p) const = default;
};

enum Node : uint8_t
{
    UNVISITED   = 0,
    WALL        = 1,
    START       = 2,
    END         = 3,
    EXPANDED    = 4,
    EXAMINED    = 5,
    PATH        = 6
};

struct State
{
    std::vector<Node> map;
    int width, height;

    Point begin;
    Point end;

    std::string map_name;
};

#endif