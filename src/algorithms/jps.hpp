#ifndef JPS_HPP
#define JPS_HPP

#include "algorithms/algorithm.hpp"

class JumpPointSearch : public Algorithm
{
public:

    void init(State* state);
    Algorithm::Result::Type update();
    Algorithm::Result get_result();

private:

};

#endif