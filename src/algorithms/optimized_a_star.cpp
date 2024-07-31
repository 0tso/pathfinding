#include "algorithms/optimized_a_star.hpp"
#include "algorithms/util.hpp"
#include "state.hpp"

#include <iostream>

void OptimizedAStar::init(State* s)
{
    curr_run_id++;
    state = s;
    lowest_path = std::numeric_limits<float>::infinity();
    best_start_to_mid_node = NULL_NODE_IDX;
    best_end_to_mid_node   = NULL_NODE_IDX;

    result = Algorithm::Result{};
    open_1.clear();
    open_2.clear();

    // Initialize the nodes vector.
    if(nodes.capacity() != s->map.size())
    {
        nodes = std::vector<InternalNode>(s->map.size());
    }
    
    auto start_index = Util::flatten(s->width, s->begin.x, s->begin.y);
    Util::lazy_initialize(curr_run_id, nodes[start_index]);
    nodes[start_index].distance_1 = 0.0f;
    open_1.push(Util::diagonal_distance(state->begin.x, state->begin.y, state->end.x, state->end.y), start_index);

    auto end_index = Util::flatten(s->width, s->end.x, s->end.y);
    Util::lazy_initialize(curr_run_id, nodes[end_index]);
    nodes[end_index].distance_2 = 0.0f;
    open_2.push(Util::diagonal_distance(state->end.x, state->end.y, state->begin.x, state->begin.y), end_index);
}

Algorithm::Result::Type OptimizedAStar::update()
{
    for(bool start : {true, false})
    {
        // Most of the time, only 1 loop. "break" at the end of scope.
        while(true)
        {
            if(open_1.empty() || open_2.empty())
            {
                // End of execution
                if(lowest_path == std::numeric_limits<float>::infinity())
                {
                    result.type = Result::Type::FAILURE;
                    return Result::Type::FAILURE;
                }

                Util::format_bidirectional_nodes(&nodes[0], best_start_to_mid_node, best_end_to_mid_node);
                Util::build_path(*state, &nodes[0], result);
                result.length = lowest_path;
                result.type = Result::Type::SUCCESS;
                return result.type;
            }

            auto& open          = start ? open_1                    : open_2;
            auto dist_ptr       = start ? &InternalNode::distance_1 : &InternalNode::distance_2;
            auto other_dist_ptr = start ? &InternalNode::distance_2 : &InternalNode::distance_1;
            auto other_top      = start ? open_2.top().value        : open_1.top().value;

            auto heuristic = [this](bool s, int x, int y) -> float
            {
                if(s)
                    return Util::diagonal_distance(x, y, this->state->end.x, this->state->end.y);
                else
                    return Util::diagonal_distance(x, y, this->state->begin.x, this->state->begin.y);
            };

            auto [approx_dist, node_idx] = open.pop();

            auto [x, y] = Util::expand(state->width, node_idx);
            auto& node = nodes[node_idx];

            if(node.status == InternalNode::EXAMINED)
            {
                // Skip this node by looping again
                open.update_write();
                continue;
            }

            node.status = InternalNode::EXAMINED;
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
                
                float new_dist = node.*dist_ptr + (dir->straight ? 1.0f : SQRT_2);
                
                if(new_dist + neighbour.*other_dist_ptr < lowest_path)
                {
                    best_start_to_mid_node = start ? node_idx      : neighbour_idx;
                    best_end_to_mid_node   = start ? neighbour_idx : node_idx;
                    lowest_path            = new_dist + neighbour.*other_dist_ptr;
                }

                if(new_dist < neighbour.*dist_ptr)
                {
                    neighbour.*dist_ptr = new_dist;

                    if(neighbour.*other_dist_ptr == std::numeric_limits<float>::infinity())
                    {
                        neighbour.prev = node_idx;

                        float f = new_dist + heuristic(start, neighbour_x, neighbour_y);

                        if(!(lowest_path <= f
                        || lowest_path <= new_dist + other_top - heuristic(!start, neighbour_x, neighbour_y)))
                        {
                            neighbour.status = InternalNode::Status::UNEXAMINED;
                            open.push(f, neighbour_idx);
                            state->map[neighbour_idx] = Node::EXAMINED;
                        }
                    }
                }
            }
            open.update_write();
            break;
        }
    }

    return Result::Type::EXECUTING;
}