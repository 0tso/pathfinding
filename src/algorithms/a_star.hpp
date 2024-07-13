#ifndef A_STAR_HPP
#define A_STAR_HPP

#include <mutex>
#include <queue>
#include <vector>
#include <utility>

#include "state.hpp"
#include "algorithms/algorithm.hpp"

class AStar : public Algorithm
{
public:

    void init(State* state);
    Algorithm::Result::Type update();
    Algorithm::Result get_result();

private:

    /**
     * The nodes will be referred to using node_indices.
     * The node_indices refer to the nodes vector.
     */
    typedef size_t node_index;

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
     * Heuristic function.
     * 
     * @param x The x-position of the node.
     * @param y The y-position of the node.
     */
    float heuristic(int x, int y);

    /**
     * Finds all the possible neighbours of the node in the specified location.
     * Stores all the found neighbours in the "neighbours" buffer.
     * 
     * @param x The x-position of the node.
     * @param y The y-position of the node.
     * 
     * @returns The number of neighbours found and added to the buffer.
     */
    int get_neighbours(int x, int y);

    /**
     * The buffer that the aforementioned function uses to store its values.
     * The first value of the pair is the neighbour
     * The second value of the pair is whether or not it's a diagonal neighbour
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