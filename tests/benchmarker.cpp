#include <vector>
#include <iostream>
#include <chrono>
#include <cmath>
#include <iomanip>

#include "benchmarker.hpp"
#include "main.hpp"
#include "all_algorithms.hpp"
#include "algorithms/util.hpp"

/**
 * Approximate floating point comparison.
 * Adapted from https://stackoverflow.com/a/32334103, license: CC BY-SA 4.0
 */
inline bool approx_equal(float a, float b)
{
    if(a == b)
        return true;

    static constexpr float epsilon = std::numeric_limits<float>::epsilon() * 128.0f;
    return std::abs(a - b) < std::max(std::numeric_limits<float>::min(), std::abs(a + b) * epsilon);
}


void Benchmarker::benchmark()
{
    std::cout << std::fixed << std::setprecision(1);
    std::cout << "map_name,scenario_distance,";
    for(const auto [algo_name, algo] : algorithms)
    {
        std::cout << algo_name << ",";
    }
    std::cout << std::endl;

    State* previous_state = nullptr;
    for(const auto& scenario : scenarios)
    {
        State* state = &maps[scenario.map_name];
        state->begin = scenario.start;
        state->end = scenario.end;

        std::cout << scenario.map_name
            << "," << scenario.optimal_length
            << ",";

        for(const auto [algo_name, algo] : algorithms)
        {
            if(state != previous_state)
            {
                // For allocating and preprocessing maps
                algo->init(state);
            }

            auto start = std::chrono::high_resolution_clock::now();
            algo->init(state);
            auto after_init = std::chrono::high_resolution_clock::now();
            while(algo->update() == Algorithm::Result::Type::EXECUTING)
            {

            }
            auto res = algo->get_result();
            auto end = std::chrono::high_resolution_clock::now();

            if(!approx_equal(res.length, scenario.optimal_length))
            {
                std::cout << "NON_OPTIMAL_DIFF:" << std::abs(scenario.optimal_length - res.length);
            }
            else
            {
                float total = std::chrono::duration<float, std::micro>(end - start).count();

                std::cout << total << ",";
            }
        }
        std::cout << std::endl;

        previous_state = state;
    }
}