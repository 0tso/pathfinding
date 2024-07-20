#ifndef STATE_HPP
#define STATE_HPP

#include <cstdint>
#include <vector>
#include <SFML/System/Vector2.hpp>

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

    sf::Vector2i begin;
    sf::Vector2i end;
};

#endif