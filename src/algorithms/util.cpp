#include "algorithms/util.hpp"
#include "state.hpp"


int Util::get_neighbours(std::pair<node_index, bool>* buffer, const State& state, int x, int y)
{
    /**
     * The constant buffer containing neigbour directions.
     * The first two values are the direction of the neigbour.
     * The second two values are the adjacent perpendiculars. They are only set for diagonals.
     * These adjacent perpendiculars need to be checked so that the algorithm doesn't go through walls diagonally.
     */
    static const int changes[][4] =
    {
        // Perpendiculars
        {0, 1, 0, 0},
        {1, 0, 0, 0},
        {0, -1, 0, 0},
        {-1, 0, 0, 0},
        
        // Diagonals
        {-1, 1, 0, 3},
        {1, 1, 0, 1},
        {1, -1, 1, 2},
        {-1, -1, 2, 3}
    };

    auto is_wall = [&](int x, int y) -> std::pair<bool, node_index>
    {
        node_index idx = flatten(state.width, x, y);
        return {state.map[idx] == Node::WALL, idx};
    };

    int amount_neighbours = 0;
    for(int i = 0; i < 8; ++i)
    {
        int new_x = x + changes[i][0];
        int new_y = y + changes[i][1];

        if(new_x >= 0 && new_x < state.width
        && new_y >= 0 && new_y < state.height)
        {
            auto [wall, new_index] = is_wall(new_x, new_y);
            if(!wall)
            {
                // Make sure it doesn't go through walls diagonally.
                if(i > 3)
                {
                    auto perp1 = changes[i][2];
                    auto perp2 = changes[i][3];
                    auto [wall1, _1] = is_wall(x + changes[perp1][0], y + changes[perp1][1]);
                    auto [wall2, _2] = is_wall(x + changes[perp2][0], y + changes[perp2][1]);
                    if(wall1 || wall2)
                    {
                        continue;
                    }
                }

                buffer[amount_neighbours] = {new_index, i > 3};
                amount_neighbours++;
            }
        }
    }
    return amount_neighbours;
}

std::pair<int, int> Util::get_movement(Direction dir)
{
    static const std::pair<int, int> movement[] =
    {
        {0, -1},
        {1, -1},
        {1, 0},
        {1, 1},
        {0, 1},
        {-1, 1},
        {-1, 0},
        {-1, -1}
    };

    return movement[dir];
}