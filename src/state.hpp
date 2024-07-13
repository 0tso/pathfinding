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
    VISITED     = 4,
    SAVED       = 5
};

struct State
{
    std::vector<Node> map;
    int width, height;

    sf::Vector2i begin;
    sf::Vector2i end;
};

#endif