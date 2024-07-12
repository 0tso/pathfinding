#ifndef RENDERER_HPP
#define RENDERER_HPP

#include <SFML/Graphics/RenderWindow.hpp>

#include "state.hpp"
#include "view.hpp"

namespace Renderer
{
    void render(sf::RenderWindow& window, const State& state, const View& view);
}

#endif