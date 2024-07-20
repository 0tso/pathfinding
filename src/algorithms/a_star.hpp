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
};

#endif