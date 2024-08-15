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
    nodes[start_index].distance = 0.0f;
    open_1.push(Util::diagonal_distance(state->begin.x, state->begin.y, state->end.x, state->end.y), start_index);

    auto end_index = Util::flatten(s->width, s->end.x, s->end.y);
    Util::lazy_initialize(curr_run_id, nodes[end_index]);
    nodes[end_index].distance = 0.0f;
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
            auto other_top      = start ? open_2.top().value        : open_1.top().value;
            auto EXAMINED       = start ? InternalNode::EXAMINED_1  : InternalNode::EXAMINED_2;
            auto OTHER_EXAMINED = start ? InternalNode::EXAMINED_2  : InternalNode::EXAMINED_1;
            auto RE             = start ? InternalNode::RE_1        : InternalNode::RE_2;
            auto OTHER_RE       = start ? InternalNode::RE_2        : InternalNode::RE_1;

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

            if(node.status == EXAMINED)
            {
                // Skip this node by looping again
                open.update_write();
                continue;
            }

            node.status = EXAMINED;
            result.expanded++;

            // Is the current node the first node? If not, set it to EXPANDED
            if(node.prev != NULL_NODE_IDX)
            {
                state->map[node_idx] = start ? Node::EXPANDED_1 : Node::EXPANDED_2;
            }

            std::pair<node_index, dir_t> neighbours[8];
            auto amount_neighbours = Util::get_neighbours(neighbours, *state, x, y);

            for(int i = 0; i < amount_neighbours; ++i)
            {
                auto [neighbour_idx, dir] = neighbours[i];
                auto& neighbour = nodes[neighbour_idx];
                Util::lazy_initialize(curr_run_id, neighbour);
                auto [neighbour_x, neighbour_y] = Util::expand(state->width, neighbour_idx);
                
                float new_dist = node.distance + (dir->straight ? 1.0f : SQRT_2);
                
                if(neighbour.status == OTHER_EXAMINED || neighbour.status == OTHER_RE)
                {
                    if(new_dist + neighbour.distance < lowest_path)
                    {
                        lowest_path = new_dist + neighbour.distance;
                        best_start_to_mid_node = start ? node_idx : neighbour_idx;
                        best_end_to_mid_node   = start ? neighbour_idx : node_idx;
                    }
                }
                else if(new_dist < neighbour.distance)
                {
                    neighbour.distance = new_dist;
                    neighbour.prev = node_idx;
                    float f = new_dist + heuristic(start, neighbour_x, neighbour_y);

                    if(!(lowest_path <= f
                    || lowest_path <= new_dist + other_top - heuristic(!start, neighbour_x, neighbour_y)))
                    {
                        neighbour.status = RE;
                        open.push(f, neighbour_idx);
                        state->map[neighbour_idx] = start ? Node::EXAMINED_1 : Node::EXAMINED_2;
                    }
                }

            }
            open.update_write();
            break;
        }
    }

    return Result::Type::EXECUTING;
}