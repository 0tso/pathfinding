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
    WALL,
    START,
    END,
    EXPANDED_1,
    EXPANDED_2,
    EXAMINED_1,
    EXAMINED_2,
    PATH
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