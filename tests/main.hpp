#ifndef MAIN_HPP
#define MAIN_HPP

#include <string>
#include <vector>
#include <unordered_map>

#include "state.hpp"

struct Scenario
{
    std::string map_name;
    Point start, end;
    float optimal_length;
    int id;
};

extern std::vector<Scenario> scenarios;
extern std::unordered_map<std::string, State> maps;

#endif