#include <iostream>
#include <SFML/Graphics.hpp>

#include "state.hpp"
#include "view.hpp"
#include "renderer.hpp"

int main()
{
    sf::RenderWindow window{sf::VideoMode{2000, 1500}, "Pathfinding visualization"};
    window.setVerticalSyncEnabled(true);

    State state = {
        .map = {},
        .width = 500,
        .height = 500
    };

    View view = {
        .movement = {0.0, 0.0},
        .zoom = 1.0f
    };

    for(int i = 0; i < state.width * state.height; ++i)
    {
        auto rand = i % 6;
        state.map.push_back((Node)rand);
    }

    while(window.isOpen())
    {
        sf::Event event;
        sf::Clock clock;
        while(window.pollEvent(event))
        {
            float delta = clock.restart().asSeconds();

            if(event.type == sf::Event::Closed)
            {
                window.close();
            }
            
            window.clear();

            Renderer::render(window, state, view);

            window.display();
        }
    }

    return 0;
}