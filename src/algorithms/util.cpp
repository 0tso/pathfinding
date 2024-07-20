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



int Util::get_neighbours(std::pair<node_index, dir_t>* buffer, const State& state, int x, int y)
{
    /**
     * The first value is the direction of the neighbour.
     * The second two values are the adjacent perpendiculars. They are only set for diagonals.
     * These adjacent perpendiculars need to be checked so that the algorithm doesn't go through walls diagonally.
     */
    static const std::tuple<dir_t, int, int> changes[] =
    {
        // Perpendiculars = straight directions
        {DIR_SOUTH, 0, 0},
        {DIR_EAST, 0, 0},
        {DIR_NORTH, 0, 0},
        {DIR_WEST, 0, 0},
        
        // Diagonals
        {DIR_SOUTHWEST, 0, 3},
        {DIR_SOUTHEAST, 0, 1},
        {DIR_NORTHEAST, 1, 2},
        {DIR_NORTHWEST, 2, 3}
    };

    auto wall_check = [&](int x, int y) -> std::pair<bool, node_index>
    {
        node_index idx = flatten(state.width, x, y);
        return {is_empty(state, x, y), idx};
    };

    int amount_neighbours = 0;
    for(int i = 0; i < 8; ++i)
    {
        dir_t dir = std::get<dir_t>(changes[i]);
        int new_x = x + dir->movement.first;
        int new_y = y + dir->movement.second;

        auto [valid, new_index] = wall_check(new_x, new_y);
        if(valid)
        {
            // Make sure it doesn't go through walls diagonally.
            if(i > 3)
            {
                int perp1 = std::get<1>(changes[i]);
                int perp2 = std::get<2>(changes[i]);

                auto dir_1 = std::get<dir_t>(changes[perp1]);
                auto dir_2 = std::get<dir_t>(changes[perp2]);

                auto [valid1, _1] = wall_check(x + dir_1->movement.first, y + dir_1->movement.second);
                auto [valid2, _2] = wall_check(x + dir_2->movement.first, y + dir_2->movement.second);
                if(!valid1 || !valid2)
                {
                    continue;
                }
            }

            buffer[amount_neighbours] = {new_index, dir};
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