#ifndef COMMON_HPP
#define COMMON_HPP

#include <limits>
#include <queue>

#include "algorithms/algorithm.hpp"
#include "algorithms/util.hpp"

/**
 * The node implementation used internally by search algorithms.
 * 
 * Each node in the network has a:
 *  -lowest seen distance from the beginning
 *  -index of the neighbour that is closest to the beginning
 *  -status that is needed for skipping already examined nodes in the priority queue
 * 
 * In addition, for performance reasons, each node has a "last_run" value.
 * This value is equal to the last pathfinding task / run index when this node was accessed.
 * The value exists so that the "nodes" vector does not have to be completely initialized on every new pathfinding task.
 * On even slightly larger maps, this initialization step can take multiple orders of magnitude more time than the pathfinding itself.
 * See the CommonAlgorithm member function "check_node_initialized" below.
 */
struct InternalNode
{
    enum Status : uint8_t
    {
        UNEXAMINED,
        EXAMINED
    };

    float distance      = std::numeric_limits<float>::infinity(); 
    node_index prev     = NULL_NODE_IDX;
    uint32_t last_run   = 1;
    Status status       = Status::UNEXAMINED;
};

class CommonAlgorithm : public Algorithm
{
public:
    void init(State* state);
    virtual Result::Type update() = 0;

protected:
    /**
     * The main storage for Nodes. All nodes will saved here and referenced using pointers.
     */
    std::vector<InternalNode> nodes;

    /**
     * The open set.
     */
    typedef std::pair<float, node_index> queue_pair;
    std::priority_queue<queue_pair, std::vector<queue_pair>, std::greater<queue_pair>> open;


    // See the next function for details about this variable.
    uint32_t curr_run_id = 0;

    /**
     * Checks if the current node is initialized.
     * If the node's "last_run" variable doesn't match curr_run_id, sets the node to its default-initialized state.
     * This function exists so that the "nodes" vector does not have to be initialized on every run.
     * 
     * @param node The node to check
     */
    inline void check_node_initialized(InternalNode& node)
    {
        if(node.last_run != curr_run_id)
        {
            node = InternalNode{.last_run = curr_run_id};
        }
    }
};

#endif