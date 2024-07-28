#include "all_algorithms.hpp"
#include "algorithms/a_star.hpp"
#include "algorithms/jps.hpp"
#include "algorithms/bbfs.hpp"
#include "algorithms/optimized_a_star.hpp"

std::map<std::string, Algorithm*> algorithms =
{
    {"A*", new AStar()},
    {"JPS", new JumpPointSearch()},
    {"BBFS", new BBFS()},
    {"OptimizedA*", new OptimizedAStar()},
};