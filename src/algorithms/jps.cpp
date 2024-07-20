#include "algorithms/jps.hpp"
#include "state.hpp"

#include <utility>
#include <iostream>

/**
 * A matrix of relative positions.
 * The positions are used to determine whether a node has forced neigbours.
 * The index into the array is a Direction.type.
 * Only defined for straight directions, as only they can have forced neigbours.
 * 
 * Usage:
 * The node pair for each direction is a {WALL, EMPTY} pair.
 * If the WALL direction is a wall, and the EMPTY direction is empty, the node has a forced neighbour.
 */
static std::pair<dir_t, dir_t> const forced[][2] =
{
    {{DIR_SOUTHWEST, DIR_WEST}, {DIR_SOUTHEAST, DIR_EAST}},
    {},
    {{DIR_NORTHWEST, DIR_NORTH}, {DIR_SOUTHWEST, DIR_SOUTH}},
    {},
    {{DIR_NORTHWEST, DIR_WEST}, {DIR_NORTHEAST, DIR_EAST}},
    {},
    {{DIR_NORTHEAST, DIR_NORTH}, {DIR_SOUTHEAST, DIR_SOUTH}},
    {}
};

Algorithm::Result::Type JumpPointSearch::update()
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

    if(x == state->end.x && y == state->end.y)
    {
        Util::build_path<InternalNode>(*state, nodes, result);
        result.length = node.distance;
        result.type = Result::Type::SUCCESS;
        return Result::Type::SUCCESS;
    }

    if(node.status == InternalNode::Status::EXAMINED)
    {
        // Skip this node by calling the update()-function again
        return update();
    }

    node.status = InternalNode::Status::EXAMINED;
    result.expanded++;

    // Is the current node the first node? If not, set it to EXPANDED
    if(node.prev != NULL_NODE_IDX)
    {
        state->map[node_idx] = Node::EXPANDED;
    }

    std::pair<node_index, dir_t> neighbours[8];
    auto amount_neighbours = Util::get_neighbours(neighbours, *state, x, y);

    for(int i = 0; i < amount_neighbours; ++i)
    {
        auto neighbour_idx = neighbours[i].first;
        auto dir = neighbours[i].second;

        jump(node_idx, dir, node.distance);
    }

    return Result::Type::EXECUTING;
}

void JumpPointSearch::jump(node_index prev, dir_t dir, float distance)
{
    auto [old_x, old_y] = Util::expand(state->width, prev);

    int x = old_x + dir->movement.first;
    int y = old_y + dir->movement.second;
    auto node_idx = Util::flatten(state->width, x, y);
    auto& node = nodes[node_idx];

    distance += dir->straight ? 1.0f : SQRT_2;
    if(distance >= node.distance)
    {
        return;
    }

    node.distance = distance;
    node.prev = prev;
    node.status == InternalNode::Status::UNEXAMINED;


    // A lambda function to add the current node to the open queue
    auto add_to_open = [&]()
    {
        float approx_distance = node.distance + Util::euclidian_distance(x, y, state->end.x, state->end.y);
        open.emplace(approx_distance, node_idx);
    };

    if(x == state->end.x && y == state->end.y)
    {
        add_to_open();
        return;
    }

    state->map[node_idx] = Node::EXAMINED;

    if(dir->straight)
    {
        // Check for forced neigbours
        for(auto [wall_dir, empty_dir] : forced[dir->type])
        {
            auto wall_x = x + wall_dir->movement.first;
            auto wall_y = y + wall_dir->movement.second;
            auto empty_x = x + empty_dir->movement.first;
            auto empty_y = y + empty_dir->movement.second;
            if(Util::is_wall(*state, wall_x, wall_y)
            && Util::is_empty(*state, empty_x, empty_y))
            {
                // Forced neighbour found
                add_to_open();
                return;
            }
        }
    } else
    {
        for(dir_t component : {dir->components.first, dir->components.second})
        {
            if(Util::is_move_valid(*state, x, y, component))
                jump(node_idx, component, distance);
        }
    }

    // Keep jumping in this direction
    if(Util::is_move_valid(*state, x, y, dir))
        jump(node_idx, dir, distance);
}