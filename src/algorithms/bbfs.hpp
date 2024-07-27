#ifndef BFS_HPP
#define BFS_HPP

#include <vector>
#include <queue>
#include "algorithms/algorithm.hpp"
#include "algorithms/util.hpp"

struct BBFSInternal
{
    enum Status : uint8_t
    {
        UNSEARCHED      = 0,
        SEARCHED_START  = 1,
        SEARCHED_END    = 2
    };

    node_index prev = NULL_NODE_IDX;
    float distance = std::numeric_limits<float>::max();
    uint32_t last_run = 0;

    Status status = UNSEARCHED;
};

/**
 * Bidirectional Breadth-First Search
 */
class BBFS : public Algorithm
{
public:
    void init(State* state);
    Result::Type update();

private:
    uint32_t current_run_id = 0;

    std::vector<BBFSInternal> nodes;

    std::queue<node_index> start_queue;
    std::queue<node_index> end_queue;

    inline void check_node_initialized(BBFSInternal& node)
    {
        if(node.last_run != current_run_id)
        {
            node = BBFSInternal{.last_run = current_run_id};
        }
    }

    node_index best_start_to_mid_node = NULL_NODE_IDX;
    node_index best_end_to_mid_node   = NULL_NODE_IDX;
    float best_path_distance = std::numeric_limits<float>::infinity();
    float lowest_start_distance;
    float lowest_end_distance;

    void format_path_nodes();
};

#endif