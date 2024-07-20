#include "algorithms/util.hpp"
#include "state.hpp"

#include <tuple>

/**
 * Definitions for directions.
 */

Direction _n = {Direction::NORTH, true, {0, -1}};
Direction _e = {Direction::EAST,  true, {1, 0}};
Direction _s = {Direction::SOUTH, true, {0, 1}};
Direction _w = {Direction::WEST,  true, {-1, 0}};

Direction _ne = {Direction::NORTHEAST, false, {1, -1}, {&_n, &_e}};
Direction _se = {Direction::SOUTHEAST, false, {1, 1}, {&_s, &_e}};
Direction _sw = {Direction::SOUTHWEST, false, {-1, 1}, {&_s, &_w}};
Direction _nw = {Direction::NORTHWEST, false, {-1, -1}, {&_n, &_w}};

dir_t const directions[] =
{
    &_n,
    &_ne,
    &_e,
    &_se,
    &_s,
    &_sw,
    &_w,
    &_nw
};
dir_t const DIR_NORTH     = directions[0];
dir_t const DIR_NORTHEAST = directions[1];
dir_t const DIR_EAST      = directions[2];
dir_t const DIR_SOUTHEAST = directions[3];
dir_t const DIR_SOUTH     = directions[4];
dir_t const DIR_SOUTHWEST = directions[5];
dir_t const DIR_WEST      = directions[6];
dir_t const DIR_NORTHWEST = directions[7];


bool Util::is_move_valid(const State& state, int x, int y, dir_t dir)
{
    int new_x = x + dir->movement.first;
    int new_y = y + dir->movement.second;

    if(!is_empty(state, new_x, new_y))
    {
        return false;
    }

    if(!dir->straight)
    {
        // Check the components
        for(auto component : {dir->components.first, dir->components.second})
        {
            int chk_x = x + component->movement.first;
            int chk_y = y + component->movement.second;
            if(!is_empty(state, chk_x, chk_y))
            {
                return false;
            }
        }
    }
    
    return true;
}


int Util::get_neighbours(std::pair<node_index, dir_t>* buffer, const State& state, int x, int y)
{
    int amount_neighbours = 0;
    for(int i = 0; i < 8; ++i)
    {
        dir_t dir = directions[i];
        if(is_move_valid(state, x, y, dir))
        {
            int new_x = x + dir->movement.first;
            int new_y = y + dir->movement.second;
            auto new_idx = flatten(state.width, new_x, new_y);

            buffer[amount_neighbours] = {new_idx, dir};
            amount_neighbours++;
        }
    }
    return amount_neighbours;
}

const Direction* Util::get_direction(int x1, int y1, int x2, int y2)
{
    int x_d = std::clamp(x2 - x1, -1, 1);
    int y_d = std::clamp(y2 - y1, -1, 1);

    if(x_d > 0)
    {
        return directions[Direction::EAST + y_d];
    }
    if(x_d == 0)
    {
        return y_d > 0 ? directions[Direction::SOUTH] : directions[Direction::NORTH];
    }

    // d_x < 0
    return directions[Direction::WEST - y_d];
}