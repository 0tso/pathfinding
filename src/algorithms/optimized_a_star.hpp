#ifndef OPTIMIZED_A_STAR_HPP
#define OPTIMIZED_A_STAR_HPP

#include <string>

#include "algorithms/util.hpp"
#include "algorithms/algorithm.hpp"
#include "algorithms/bucket_queue.hpp"

class OptimizedAStar : public Algorithm
{
public:
    void init(State* state);
    Result::Type update();

private:
    struct InternalNode
    {
        enum Status : uint8_t
        {
            FREE,
            EXAMINED_1,
            EXAMINED_2,
            RE_1,
            RE_2
        };

        node_index prev   = NULL_NODE_IDX;
        float distance    = std::numeric_limits<float>::infinity(); 
        uint32_t last_run = 1;
        Status status     = Status::FREE;
    };

    std::vector<InternalNode> nodes;
    BucketQueue<node_index> open_1{0.1f, 30, 5};
    BucketQueue<node_index> open_2{0.1f, 30, 5};
    uint32_t curr_run_id;

    float lowest_path = std::numeric_limits<float>::infinity();
    node_index best_start_to_mid_node = NULL_NODE_IDX;
    node_index best_end_to_mid_node   = NULL_NODE_IDX;

};

#endif