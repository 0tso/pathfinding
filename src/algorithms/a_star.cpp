#include <cmath>
#include <algorithm>
#include <utility>

#include "algorithms/a_star.hpp"
#include "state.hpp"

/**
 * Flattens the input coordinates (x, y) to single-dimensional array coordinates.
 */
int flatten(int width, int x, int y)
{
    return y * width + x;
}

void AStar::init(State* s)
{
    state = s;

    nodes.clear();
    while(!open.empty())
        open.pop();

    // Initialize the nodes vector.
    for(int i = 0; i < s->map.size(); ++i)
    {
        nodes.emplace_back();
    }
    
    // Set the correct information of the starting node and add it to the open queue.
    auto start_index = flatten(s->width, s->begin.x, s->begin.y);
    nodes[start_index].distance = 0.0f;
    open.emplace(0.0f, start_index);
}

float AStar::heuristic(int x, int y)
{
    // Manhattan distance
    return std::abs(state->end.x - x) + std::abs(state->end.y - y);
}

int AStar::get_neighbours(int x, int y)
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
        node_index idx = flatten(state->width, x, y);
        return {state->map[idx] == Node::WALL, idx};
    };

    int amount_neighbours = 0;
    for(int i = 0; i < 8; ++i)
    {
        int new_x = x + changes[i][0];
        int new_y = y + changes[i][1];

        if(new_x >= 0 && new_x < state->width
        && new_y >= 0 && new_y < state->height)
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

                neighbours[amount_neighbours] = {new_index, i > 3};
                amount_neighbours++;
            }
        }
    }
    return amount_neighbours;
}

Algorithm::Result::Type AStar::update()
{
    if(open.empty())
    {
        result.type = Result::Type::FAILURE;
        return Result::Type::FAILURE;
    }

    auto [approx_dist, node_idx] = open.top();
    open.pop();

    int x = node_idx % state->width;
    int y = node_idx / state->width;

    auto& node = nodes[node_idx];

    if(node.status == InternalNode::Status::EXAMINED)
    {
        // Skip this node by calling the update()-function again
        return update();
    }

    node.status = InternalNode::Status::EXAMINED;
    result.expanded++;

    if(node.prev != std::numeric_limits<node_index>::infinity())
    {
        state->map[node_idx] = Node::VISITED;
    }

    auto amount_neighbours = get_neighbours(x, y);
    for(int i = 0; i < amount_neighbours; ++i)
    {
        auto [neighbour_idx, diagonal] = neighbours[i];
        auto& neighbour     = nodes[neighbour_idx];
        auto neighbour_x    = neighbour_idx % state->width;
        auto neighbour_y    = neighbour_idx / state->width;
        
        // All perpendicular neighbours are one unit of distance away,
        // and the diagonal members are sqrt(2) units of distance away.
        float new_dist = node.distance + (diagonal ? 1.414213f : 1.0f);
        if(new_dist < neighbour.distance)
        {

            // Check if the neigbour is the end
            if(neighbour_x == state->end.x && neighbour_y == state->end.y)
            {
                // End & path found!
                result.type = Result::Type::SUCCESS;
                result.path.emplace_back(neighbour_x, neighbour_y);

                // Construct the path
                node_index prev_idx = node_idx;
                while(true)
                {
                    auto& prev_node = nodes[prev_idx];
                    if(prev_node.prev == std::numeric_limits<node_index>::infinity())
                    {
                        break;
                    }
                    x = prev_idx % state->width;
                    y = prev_idx / state->width;
                    result.path.emplace_back(x, y);
                    state->map[prev_idx] = Node::PATH;
                    prev_idx = prev_node.prev;
                }

                std::reverse(result.path.begin(), result.path.end());
                result.length = result.path.size();

                return Result::Type::SUCCESS;
            }
            else
            {
                state->map[neighbour_idx] = Node::SAVED;
            }

            // We have found a new, more optimized way to reach this node.
            // Therefore we should expand this node again later.

            neighbour.distance = new_dist;
            neighbour.status = InternalNode::Status::UNEXAMINED;
            neighbour.prev = node_idx;
            float approx_total_path_length = new_dist + heuristic(neighbour_x, neighbour_y);
            open.emplace(approx_total_path_length, neighbour_idx);
        }
    }

    return Result::Type::EXECUTING;
}

Algorithm::Result AStar::get_result()
{
    return result;
}