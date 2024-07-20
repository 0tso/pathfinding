#include "algorithms/common.hpp"

void CommonAlgorithm::init(State* s)
{
    state = s;

    nodes.clear();
    while(!open.empty())
        open.pop();

    // Initialize the nodes vector.
    for(int i = 0; i < s->map.size(); ++i)
    {
        nodes.emplace_back();
    }
    
    // Set the correct information of the starting node and add it to the open queue.
    auto start_index = Util::flatten(s->width, s->begin.x, s->begin.y);
    nodes[start_index].distance = 0.0f;
    open.emplace(0.0f, start_index);
}