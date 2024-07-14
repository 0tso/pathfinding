#ifndef A_STAR_HPP
#define A_STAR_HPP

#include <mutex>
#include <queue>
#include <vector>
#include <utility>

#include "state.hpp"
#include "algorithms/algorithm.hpp"
#include "algorithms/util.hpp"

class AStar : public Algorithm
{
public:

    void init(State* state);
    Algorithm::Result::Type update();
    Algorithm::Result get_result();

private:


    /**
     * The node implementation used internally.
     * 
     * Each node in the network has a:
     *  -lowest seen distance from the beginning
     *  -index of the neighbour that is closest to the beginning
     *  -status that is needed for skipping already examined nodes in the priority queue
     */
    struct InternalNode
    {
        enum Status
        {
            UNEXAMINED,
            EXAMINED
        };

        float distance      = std::numeric_limits<float>::infinity(); 
        node_index prev     = std::numeric_limits<node_index>::infinity();
        Status status       = Status::UNEXAMINED;
    };

    /**
     * The buffer in which to store neighbouring nodes.
     * A pair of noded index and a bool specifying whether or not the node is a diagonal neighbour.
     */
    std::pair<node_index, bool> neighbours[8];

    /**
     * The main storage for Nodes. All nodes will saved here and referenced using pointers.
     */
    std::vector<InternalNode> nodes;

    /**
     * The open set.
     */
    typedef std::pair<float, node_index> queue_pair;
    std::priority_queue<queue_pair, std::vector<queue_pair>, std::greater<queue_pair>> open;

    /**
     * The variable for storing result information.
     */
    Result result;
};

#endif