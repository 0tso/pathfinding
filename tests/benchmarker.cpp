#include <vector>
#include <iostream>
#include <chrono>
#include <cmath>
#include <iomanip>

#include "benchmarker.hpp"
#include "main.hpp"
#include "all_algorithms.hpp"
#include "algorithms/util.hpp"

inline bool approx_equal(float a, float b)
{
    auto a_abs = std::abs(a);
    auto b_abs = std::abs(b);
    return std::abs(a - b) <= ((a_abs < b_abs ? b_abs : a_abs) * 0.0001f);
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
                std::cout << "ERROR: fails for " << algo_name << " which returns " << res.length << std::endl;
                return;
            }

            float total = std::chrono::duration<float, std::micro>(end - start).count();

            std::cout << total << ",";
        }
        std::cout << std::endl;
    }
}