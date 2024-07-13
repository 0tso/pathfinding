#ifndef A_STAR_HPP
#define A_STAR_HPP

#include <mutex>

#include "state.hpp"
#include "algorithms/algorithm.hpp"

class AStar : public Algorithm
{
public:

    void init(State* state);
    Algorithm::Result::Type update();
    Algorithm::Result get_result();

private:
    int x, y;
};

#endif