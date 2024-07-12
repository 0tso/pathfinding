#include <SFML/Graphics.hpp>

#include <vector>

#include "renderer.hpp"
#include "state.hpp"
#include "view.hpp"

namespace Renderer
{
    constexpr int DEFAULT_PIXEL_SIZE = 2;

    std::vector<sf::Vertex> vertices;

    sf::Vertex quad[] = {
        sf::Vector2f{0.0f, 0.0f},
        sf::Vector2f{0.0f, 1.0f},
        sf::Vector2f{1.0f, 1.0f},

        sf::Vector2f{0.0f, 0.0f},
        sf::Vector2f{1.0f, 1.0f},
        sf::Vector2f{1.0f, 0.0f}
    };

    sf::Color node_colors[] = {
        sf::Color::White,
        sf::Color::Black,
        sf::Color::Blue,
        sf::Color::Red,
        sf::Color{50, 50, 50},
        sf::Color{100, 100, 100}
    };

    void render(sf::RenderWindow& window, const State& state, const View& view)
    {
        const auto vert_amount = state.height * state.width * 6;

        // Fill the vertex buffer if it's not big enough
        if(vertices.size() != vert_amount)
        {
            vertices = std::vector<sf::Vertex>(vert_amount);
        }

        float pixel_size = DEFAULT_PIXEL_SIZE * view.zoom;
        sf::Vector2f transform = view.movement;

        // Assign the vertex data
        for(int i = 0; i < state.width * state.height; ++i)
        {
            int y = i / state.width;
            int x = i % state.width;

            auto color = node_colors[state.map[i]];
            for(int v = 0; v < 6; ++v)
            {
                int index = i * 6 + v;
                float v_x = (quad[v].position.x + x) * pixel_size + transform.x;
                float v_y = (quad[v].position.y + y) * pixel_size + transform.y;
                vertices[index] = {sf::Vector2f{v_x, v_y}, color};
            }
        }

        window.draw(&vertices[0], vertices.size(), sf::Triangles);
    }
}