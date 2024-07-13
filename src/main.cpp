#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <mutex>
#include <thread>
#include <functional>
#include <string>
#include <atomic>
#include <cmath>

#include "state.hpp"
#include "renderer.hpp"

State global_state;
bool pathfinding = false;


sf::Vector2i get_mouse_world_coords(const sf::RenderWindow& window)
{
    auto mouse_pos = sf::Mouse::getPosition(window);
    auto world_coords = window.mapPixelToCoords(mouse_pos);
    int x = (int)world_coords.x;
    int y = (int)world_coords.y;
    return {x, y};
}

bool check_coords(const State& state, int x, int y)
{
    return x >= 0 && x < state.width && y >= 0 && y < state.height;
}

void render_loop(sf::RenderWindow* window, State* state, std::mutex* mut)
{
    window->setActive(true);
    window->setKeyRepeatEnabled(false);
    sf::View view = window->getDefaultView();

    view.setCenter(100, 100);
    view.zoom(0.01);
    window->setView(view);

    auto set_node = [&](int x, int y, Node type)
    {
        if(!pathfinding && check_coords(*state, x, y))
        {
            int index = y * global_state.width + x;
            global_state.map[index] = type;
            state->map[index] = type;
        }
    };

    sf::Clock clock;
    while(window->isOpen())
    {
        float delta = clock.restart().asSeconds();

        sf::Event event;
        while(window->pollEvent(event))
        {
            switch(event.type)
            {
                case sf::Event::Closed:
                {
                    window->close();
                    break;
                }
                case sf::Event::MouseWheelScrolled:
                {
                    auto amount = std::pow(2, -event.mouseWheelScroll.delta * delta * 100.0f);
                    view.zoom(amount);
                    window->setView(view);
                    break;
                }
                case sf::Event::Resized:
                {
                    view.setSize({
                        (float)event.size.width,
                        (float)event.size.height
                    });
                    window->setView(view);
                    break;
                }
                case sf::Event::KeyPressed:
                {
                    if(event.key.code == sf::Keyboard::Q)
                    {
                        auto [x, y] = get_mouse_world_coords(*window);
                        set_node(global_state.begin.x, global_state.begin.y, Node::UNVISITED);
                        set_node(x, y, Node::START);
                        global_state.begin = {x, y};
                    }
                    if(event.key.code == sf::Keyboard::E)
                    {
                        auto [x, y] = get_mouse_world_coords(*window);
                        set_node(global_state.end.x, global_state.end.y, Node::UNVISITED);
                        set_node(x, y, Node::END);
                        global_state.end = {x, y};
                    }
                }
            }
        }

        if(window->hasFocus())
        {
            if(sf::Keyboard::isKeyPressed(sf::Keyboard::A))
                view.move(-delta * 100.0f, 0);
            if(sf::Keyboard::isKeyPressed(sf::Keyboard::D))
                view.move(delta * 100.0f, 0);
            if(sf::Keyboard::isKeyPressed(sf::Keyboard::W))
                view.move(0, -delta * 100.0f);
            if(sf::Keyboard::isKeyPressed(sf::Keyboard::S))
                view.move(0, delta * 100.0f);
            window->setView(view);

            if(sf::Mouse::isButtonPressed(sf::Mouse::Left))
            {
                auto [x, y] = get_mouse_world_coords(*window);
                set_node(x, y, Node::WALL);
            }
            if(sf::Mouse::isButtonPressed(sf::Mouse::Right))
            {
                auto [x, y] = get_mouse_world_coords(*window);
                set_node(x, y, Node::UNVISITED);
            }
        }

        window->clear(sf::Color{72, 13, 54});
        mut->lock();
        Renderer::render(*window, *state);
        mut->unlock();
        window->display();
    }
}

void pathfinding_loop(std::function<void(void)> algo,
                        State* state, State* render_state,
                        std::mutex render_mut, std::chrono::duration<int, std::ratio<1, 1000>> sleep_time)
{

}

int main()
{
    sf::RenderWindow window{sf::VideoMode{2000, 1500}, "Pathfinding visualization"};
    window.setVerticalSyncEnabled(true);
    window.setActive(false);

    global_state = {
        .map = {},
        .width = 250,
        .height = 250
    };

    for(int i = 0; i < global_state.width * global_state.height; ++i)
    {
        global_state.map.push_back(Node::UNVISITED);
    }

    State render_state{global_state};
    std::mutex render_update_mutex;
    std::thread render{render_loop, &window, &render_state, &render_update_mutex};

    while(window.isOpen())
    {
        std::cout << "> ";

        std::string i;
        std::getline(std::cin, i);

        auto get_next_token = [&]() -> std::string
        {
            auto pos = i.find(' ');
            if(pos == std::string::npos)
                return i;
            else
            {
                auto token = i.substr(0, pos);
                i.erase(0, pos + 1);
                return token;
            }
        };

        auto first = get_next_token();
        if(first == "exit")
        {
            window.close();
            render.join();
        }
    }

    return 0;
}