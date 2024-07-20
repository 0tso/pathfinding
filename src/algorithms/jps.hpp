#ifndef JPS_HPP
#define JPS_HPP

#include <queue>

#include "algorithms/common.hpp"

class JumpPointSearch : public CommonAlgorithm
{
public:
    Algorithm::Result::Type update();

private:
    
    /**
     * The "jump" function as defined in Harabor and Grastien, 2011 ("Online Graph Pruning for Pathfinding on Grid Maps"),
     * with some modifications.
     * 
     * @param node The node from which to jump forward
     * @param dir The direction of the jump
     * @param distance The distance from the beginning so far
     */
    void jump(node_index node, dir_t dir, float distance);
};

#endif