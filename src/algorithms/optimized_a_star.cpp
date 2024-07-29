#include "algorithms/optimized_a_star.hpp"
#include "algorithms/util.hpp"
#include "state.hpp"

#include <iostream>

void OptimizedAStar::init(State* s)
{
    curr_run_id++;
    state = s;

    result = Algorithm::Result{};
    open = BucketQueue<node_index>(0.1f, 30, 5);

    // Initialize the nodes vector.
    if(nodes.capacity() != s->map.size())
    {
        nodes = std::vector<InternalNode>(s->map.size());
    }
    
    // Set the correct information of the starting node and add it to the open queue.
    auto start_index = Util::flatten(s->width, s->begin.x, s->begin.y);
    Util::lazy_initialize(curr_run_id, nodes[start_index]);
    nodes[start_index].distance = 0.0f;
    open.push(Util::euclidian_distance(state->begin.x, state->begin.y, state->end.x, state->end.y), start_index);
}

Algorithm::Result::Type OptimizedAStar::update()
{
    if(open.empty())
    {
        result.type = Result::Type::FAILURE;
        return Result::Type::FAILURE;
    }

    auto [approx_dist, node_idx] = open.pop();

    auto [x, y] = Util::expand(state->width, node_idx);
    auto& node = nodes[node_idx];

    if(node.status == InternalNode::Status::EXAMINED)
    {
        // Skip this node by calling the update()-function again
        open.update_write();
        return update();
    }

    node.status = InternalNode::Status::EXAMINED;
    result.expanded++;

    // Is the current node the first node? If not, set it to EXPANDED
    if(node.prev != std::numeric_limits<node_index>::infinity())
    {
        state->map[node_idx] = Node::EXPANDED;
    }

    std::pair<node_index, dir_t> neighbours[8];
    auto amount_neighbours = Util::get_neighbours(neighbours, *state, x, y);

    for(int i = 0; i < amount_neighbours; ++i)
    {
        auto [neighbour_idx, dir] = neighbours[i];
        auto& neighbour = nodes[neighbour_idx];
        Util::lazy_initialize(curr_run_id, neighbour);
        auto [neighbour_x, neighbour_y] = Util::expand(state->width, neighbour_idx);
        
        // All perpendicular neighbours are one unit of distance away
        // and all the diagonal neigbours are sqrt(2) units of distance away.
        float new_dist = node.distance + (dir->straight ? 1.0f : SQRT_2);
        if(new_dist < neighbour.distance)
        {
            // We have found a new, more optimized way to reach this node.
            // Therefore we should expand this node again later.
            // The used heuristic is manhattan distance |x1 - x2| + |y1 - y2|.

            neighbour.distance = new_dist;
            neighbour.prev = node_idx;

            // Check if the neigbour is the end
            if(neighbour_x == state->end.x && neighbour_y == state->end.y)
            {
                // End & path found!
                Util::build_path<InternalNode>(*state, &nodes[0], result);
                result.length = neighbour.distance;
                result.type = Result::Type::SUCCESS;

                std::cout << "resized " << open.get_realloc_amount() << " times." << std::endl;
                return Result::Type::SUCCESS;
            }
            else
            {
                state->map[neighbour_idx] = Node::EXAMINED;
            }

            neighbour.status = InternalNode::Status::UNEXAMINED;
            float approx_total_path_length =
                new_dist + Util::euclidian_distance(neighbour_x, neighbour_y, state->end.x, state->end.y);
            open.push(approx_total_path_length, neighbour_idx);
        }
    }

    open.update_write();

    return Result::Type::EXECUTING;
}