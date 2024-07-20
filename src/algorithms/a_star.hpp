#ifndef A_STAR_HPP
#define A_STAR_HPP

#include <mutex>
#include <queue>
#include <vector>
#include <utility>

#include "state.hpp"
#include "algorithms/common.hpp"
#include "algorithms/util.hpp"

class AStar : public CommonAlgorithm
{
public:
    Algorithm::Result::Type update();

private:

    /**
     * The buffer in which to store neighbouring nodes.
     * A pair of noded index and a bool specifying whether or not the node is a diagonal neighbour.
     */
    std::pair<node_index, bool> neighbours[8];

};

#endif