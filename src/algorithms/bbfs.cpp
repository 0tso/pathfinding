#include "algorithms/bbfs.hpp"

void BBFS::init(State* s)
{
    state = s;
    curr_run_id++;
    result = Algorithm::Result{};

    best_start_to_mid_node = NULL_NODE_IDX;
    best_end_to_mid_node   = NULL_NODE_IDX;
    best_path_distance = std::numeric_limits<float>::infinity();

    // Initialize the nodes vector.
    if(nodes.capacity() != s->map.size())
    {
        nodes = std::vector<BBFSInternal>(s->map.size());
    }

    start_queue = std::queue<node_index>();
    end_queue   = std::queue<node_index>();

    auto start_index = Util::flatten(s->width, s->begin.x, s->begin.y);
    auto& start_node = nodes[start_index];
    Util::lazy_initialize(curr_run_id, start_node);
    start_node.distance = 0.0f;
    start_node.status = BBFSInternal::Status::SEARCHED_START;
    start_queue.push(start_index);

    auto end_index = Util::flatten(s->width, s->end.x, s->end.y);
    auto& end_node = nodes[end_index];
    Util::lazy_initialize(curr_run_id, end_node);
    end_node.distance = 0.0f;
    end_node.status = BBFSInternal::Status::SEARCHED_END;
    end_queue.push(end_index);
}

Algorithm::Result::Type BBFS::update()
{
    lowest_end_distance   = std::numeric_limits<float>::infinity();
    lowest_start_distance = std::numeric_limits<float>::infinity();

    std::pair<node_index, dir_t> neighbours[8];

    for(bool start : {true, false})
    {
        auto& q = start ? start_queue : end_queue;
        auto amount = q.size();

        for(int i = 0; i < amount; ++i)
        {
            auto node_idx = q.front();
            auto& node = nodes[node_idx];
            auto [x, y] = Util::expand(state->width, node_idx);

            if(start)
                lowest_start_distance = std::min(lowest_start_distance, node.distance);
            else
                lowest_end_distance = std::min(lowest_end_distance, node.distance);

            if(node.prev != NULL_NODE_IDX)
            {
                state->map[node_idx] = Node::EXPANDED_1;
            }

            auto amount_neighbours = Util::get_neighbours(neighbours, *state, x, y);

            for(int i = 0; i < amount_neighbours; ++i)
            {
                auto [neighbour_idx, dir] = neighbours[i];
                auto& neighbour = nodes[neighbour_idx];

                Util::lazy_initialize(curr_run_id, neighbour);

                auto [neighbour_x, neighbour_y] = Util::expand(state->width, neighbour_idx);
                float new_dist = node.distance + (dir->straight ? 1.0f : SQRT_2);

                if(!start && neighbour.status == BBFSInternal::Status::SEARCHED_START
                 || start && neighbour.status == BBFSInternal::Status::SEARCHED_END)
                {
                    float new_path_dist = new_dist + neighbour.distance;
                    if(new_path_dist < best_path_distance)
                    {
                        best_path_distance     = new_path_dist;
                        best_start_to_mid_node = start ? node_idx : neighbour_idx;
                        best_end_to_mid_node   = start ? neighbour_idx : node_idx;
                    }
                }
                else if(new_dist < neighbour.distance)
                {
                    neighbour.distance = new_dist;
                    neighbour.prev = node_idx;
                    if(neighbour.status == BBFSInternal::Status::UNSEARCHED)
                    {
                        q.push(neighbour_idx);
                        neighbour.status = start ? BBFSInternal::Status::SEARCHED_START : BBFSInternal::SEARCHED_END;

                        state->map[neighbour_idx] = Node::EXAMINED_1;
                    }
                    else
                    {
                        // Required to preserve optimality
                        recursive_update(neighbour_idx, start);
                    }
                }
            }

            q.pop();
        }
    }

    if(best_path_distance == std::numeric_limits<float>::infinity()
    && lowest_start_distance == std::numeric_limits<float>::infinity()
    && lowest_end_distance == std::numeric_limits<float>::infinity())
    {
        return Algorithm::Result::FAILURE;
    }

    if(lowest_start_distance + lowest_end_distance > best_path_distance)
    {
        // End found!
        Util::format_bidirectional_nodes(&nodes[0], best_start_to_mid_node, best_end_to_mid_node);
        Util::build_path<BBFSInternal>(*state, &nodes[0], result);
        result.length = best_path_distance;
        result.type = Result::Type::SUCCESS;

        return Algorithm::Result::SUCCESS;
    }

    return Algorithm::Result::EXECUTING;
}

void BBFS::recursive_update(node_index idx, bool start)
{
    /*
    * Uncomment to add optimality but decrease performance significantly.
    *

    std::queue<node_index> q;
    q.push(idx);
    
    std::pair<node_index, dir_t> neighbours[8];

    while(!q.empty())
    {
        auto amount = q.size();

        for(int i = 0; i < amount; ++i)
        {
            auto node_idx = q.front();
            auto& node = nodes[node_idx];
            auto [x, y] = Util::expand(state->width, node_idx);

            auto amount_neighbours = Util::get_neighbours(neighbours, *state, x, y);

            for(int i = 0; i < amount_neighbours; ++i)
            {
                auto [neighbour_idx, dir] = neighbours[i];
                auto& neighbour = nodes[neighbour_idx];

                check_node_initialized(neighbour);

                auto [neighbour_x, neighbour_y] = Util::expand(state->width, neighbour_idx);
                float new_dist = node.distance + (dir->straight ? 1.0f : SQRT_2);

                if(!start && neighbour.status == BBFSInternal::Status::SEARCHED_START
                    || start && neighbour.status == BBFSInternal::Status::SEARCHED_END)
                {
                    float new_path_dist = new_dist + neighbour.distance;
                    if(new_path_dist < best_path_distance)
                    {
                        best_path_distance     = new_path_dist;
                        best_start_to_mid_node = start ? node_idx : neighbour_idx;
                        best_end_to_mid_node   = start ? neighbour_idx : node_idx;
                    }
                }
                else if(new_dist < neighbour.distance && neighbour.status != BBFSInternal::Status::UNSEARCHED)
                {
                    neighbour.distance = new_dist;
                    neighbour.prev = node_idx;
                    q.push(neighbour_idx);
                }
            }
            q.pop();
        }
    }
    */
}