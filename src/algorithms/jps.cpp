#include "algorithms/jps.hpp"
#include "state.hpp"

#include <utility>

/**
 * A matrix of relative positions.
 * The positions are used to determine whether a node has forced neigbours.
 * The index into the array is a Direction.type.
 * Only defined for straight directions, as only they can have forced neigbours.
 * 
 * Usage:
 * If the node at the specified relative position is a wall, the node has a forced neigbour.
 */
static dir_t const forced[][2] =
{
    {DIR_SOUTHWEST, DIR_SOUTHEAST},
    {},
    {DIR_NORTHWEST, DIR_SOUTHWEST},
    {},
    {DIR_NORTHWEST, DIR_NORTHEAST},
    {},
    {DIR_NORTHEAST, DIR_SOUTHEAST},
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


}
