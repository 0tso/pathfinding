#include <cmath>
#include <algorithm>

#include "algorithms/a_star.hpp"
#include "state.hpp"

void AStar::init(State* s)
{
    this->state = s;
    this->x = s->begin.x;
    this->y = s->begin.y;
}

Algorithm::Result::Type AStar::update()
{
    auto diff_x = this->state->end.x - this->x;
    auto diff_y = this->state->end.y - this->y;
    auto mov_x = std::clamp(diff_x, -1, 1);
    auto mov_y = std::clamp(diff_y, -1, 1);

    if(std::abs(diff_x) > std::abs(diff_y))
    {
        this->x += mov_x;
    } else
    {
        this->y += mov_y;
    }
    
    auto index = this->y * this->state->width + this->x;
    if(this->state->map[index] == Node::WALL)
    {
        return Algorithm::Result::Type::FAILURE;
    }

    sf::Vector2i pos = {this->x, this->y};
    if(pos == this->state->end)
    {
        return Algorithm::Result::Type::SUCCESS;
    }


    state->map[index] = Node::SAVED;
    return Algorithm::Result::Type::EXECUTING;
}

Algorithm::Result AStar::get_result()
{
    return {
        .length = 123.0f
    };
}