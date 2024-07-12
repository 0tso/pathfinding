#ifndef VIEW_HPP
#define VIEW_HPP

#include <SFML/System/Vector2.hpp>

struct View
{
    sf::Vector2f movement = {0.0, 0.0};
    float zoom = 1;
};

#endif