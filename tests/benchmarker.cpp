#include "benchmarker.hpp"
#include "main.hpp"

#include <iostream>

void Benchmarker::benchmark()
{
    std::cout << "Benchmarking " << scenarios.size() << " scenarios. " << std::endl;
    for(auto& [key, state] : maps)
    {
        std::cout << key << " size " << state.map.size() << std::endl;
    }
}