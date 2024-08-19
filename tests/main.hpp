#ifndef MAIN_HPP
#define MAIN_HPP

#include <string>
#include <vector>
#include <unordered_map>
#include <tuple>

#include "state.hpp"
#include "algorithms/algorithm.hpp"

struct Scenario
{
    std::string map_name;
    Point start, end;
    float optimal_length;
    int id;
};

extern std::vector<std::pair<std::string, Algorithm*>> algos;

extern std::vector<Scenario> scenarios;
extern std::unordered_map<std::string, State> maps;

void load_map(const char* name, const char* full_path);

#endif