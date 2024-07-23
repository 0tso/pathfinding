#include "all_algorithms.hpp"
#include "algorithms/a_star.hpp"
#include "algorithms/jps.hpp"

std::map<std::string, Algorithm*> algorithms =
{
    {"A*", new AStar()},
    {"JPS", new JumpPointSearch()}
};