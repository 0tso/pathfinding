#ifndef RENDERER_HPP
#define RENDERER_HPP

#include <SFML/Graphics/RenderWindow.hpp>

#include "state.hpp"

namespace Renderer
{
    void render(sf::RenderWindow& window, const State& state);
}

#endif