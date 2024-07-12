#include <iostream>
#include <SFML/Graphics.hpp>

int main()
{
    sf::Window window{sf::VideoMode{1000, 700}, "Pathfinding visualization"};

    while(window.isOpen())
    {

        sf::Event event;
        while(window.pollEvent(event))
        {
            if(event.type == sf::Event::Closed)
            {
                window.close();
            }
        }
    }

    return 0;
}