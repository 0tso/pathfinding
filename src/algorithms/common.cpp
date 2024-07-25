#include "algorithms/common.hpp"
#include <cstring>

void CommonAlgorithm::init(State* s)
{
    curr_run_id++;
    state = s;

    result = Algorithm::Result{};
    open = std::priority_queue<queue_pair, std::vector<queue_pair>, std::greater<queue_pair>>();

    // Initialize the nodes vector.
    if(nodes.capacity() != s->map.size())
    {
        nodes = std::vector<InternalNode>(s->map.size());
    }
    
    // Set the correct information of the starting node and add it to the open queue.
    auto start_index = Util::flatten(s->width, s->begin.x, s->begin.y);
    check_node_initialized(nodes[start_index]);
    nodes[start_index].distance = 0.0f;
    open.emplace(0.0f, start_index);
}