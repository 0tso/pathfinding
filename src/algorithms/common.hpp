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
};