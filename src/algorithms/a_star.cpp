#include <cmath>
#include <algorithm>
#include <utility>

#include "algorithms/a_star.hpp"
#include "algorithms/util.hpp"
#include "state.hpp"

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
    auto start_index = Util::flatten(s->width, s->begin.x, s->begin.y);
    nodes[start_index].distance = 0.0f;
    open.emplace(0.0f, start_index);
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

    auto [x, y] = Util::expand(state->width, node_idx);
    auto& node = nodes[node_idx];

    if(node.status == InternalNode::Status::EXAMINED)
    {
        // Skip this node by calling the update()-function again
        return update();
    }

    node.status = InternalNode::Status::EXAMINED;
    result.expanded++;

    // Is the current node the first node? If not, set it to VISITED
    if(node.prev != std::numeric_limits<node_index>::infinity())
    {
        state->map[node_idx] = Node::VISITED;
    }

    auto amount_neighbours = Util::get_neighbours(this->neighbours, *state, x, y);
    for(int i = 0; i < amount_neighbours; ++i)
    {
        auto [neighbour_idx, is_diagonal] = neighbours[i];
        auto& neighbour = nodes[neighbour_idx];
        auto [neighbour_x, neighbour_y] = Util::expand(state->width, neighbour_idx);
        
        // All perpendicular neighbours are one unit of distance away
        // and all the diagonal neigbours are sqrt(2) units of distance away.
        float new_dist = node.distance + (is_diagonal ? 1.414214f : 1.0f);
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
                    auto [x, y] = Util::expand(state->width, prev_idx);
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
            // The used heuristic is manhattan distance |x1 - x2| + |y1 - y2|.

            neighbour.distance = new_dist;
            neighbour.status = InternalNode::Status::UNEXAMINED;
            neighbour.prev = node_idx;
            float approx_total_path_length =
                new_dist + Util::euclidian_distance(neighbour_x, neighbour_y, state->end.x, state->end.y);
            open.emplace(approx_total_path_length, neighbour_idx);
        }
    }

    return Result::Type::EXECUTING;
}

Algorithm::Result AStar::get_result()
{
    return result;
}