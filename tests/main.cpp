#include <catch2/catch_session.hpp>
#include <string>
#include <iostream>
#include <filesystem>
#include <unordered_map>
#include <utility>
#include <random>

#include "main.hpp"
#include "benchmarker.hpp"
#include "all_algorithms.hpp"
#include "hog2/ScenarioLoader.h"
#include "hog2/Map.h"

std::vector<std::pair<std::string, Algorithm*>> algos;

std::vector<Scenario> scenarios;
std::unordered_map<std::string, State> maps;

std::filesystem::path benchmark_dir;

void load_map(const char* name, const char* full_path)
{
    Map map{full_path};
    int width = map.GetMapWidth();
    int height = map.GetMapHeight();
    std::vector<Node> nodes;

    nodes.reserve(width * height);

    for(int y = 0; y < height; ++y)
    {
        for(int x = 0; x < width; ++x)
        {
            auto tile = map.GetTerrainType(x, y);
            if(tile == kGrass || tile == kGround)
                nodes.emplace_back(Node::UNVISITED);
            else
                nodes.emplace_back(Node::WALL);
        }
    }
    maps.emplace(name, State{.map = std::move(nodes), .width = width, .height = height, .map_name = name});
}

void load_scenario(const Experiment& exp, int id)
{
    auto map = exp.GetMapName();
    if(!maps.contains(map))
    {
        std::filesystem::path path = benchmark_dir / map;
        load_map(map, path.c_str());
    }

    scenarios.push_back(Scenario{
        .map_name = exp.GetMapName(),
        .start = {exp.GetStartX(), exp.GetStartY()},
        .end = {exp.GetGoalX(), exp.GetGoalY()},
        .optimal_length = (float)exp.GetDistance(),
        .id = id
    });
}

int main(int argc, char** argv)
{
    Catch::Session session;

    std::string benchmark_str;
    std::string algos_str;
    int benchmark_amount = 0;

    using namespace Catch::Clara;
    auto cli = session.cli()
        | Opt(benchmark_str, "benchmark directory")
        ["--benchmarks"]("skip the unit tests, instead read and execute benchmarks from the following directory.")
        | Opt(benchmark_amount, "benchmark amount")
        ["--amount"]("only execute this amount of benchmarking scenarios, sampled randomly from all files")
        | Opt(algos_str, "algorithms")
        ["--algorithms"]("only benchmark the specified algorithms, delimited by a comma: --algorithms A*,JPS");

    session.cli(cli);
    int ret = session.applyCommandLine(argc, argv);
    if(ret != 0)
        return ret;
    
    if(algos_str != "")
    {
        size_t idx;
        while((idx = algos_str.find(',')) != std::string::npos)
        {
            auto substr = algos_str.substr(0, idx);
            algos.emplace_back(substr, algorithms.at(substr));
            algos_str.erase(0, idx + 1);
        }
        if(algos_str != "")
        {
            algos.emplace_back(algos_str, algorithms.at(algos_str));
        }
    }
    else
    {
        algos = {algorithms.begin(), algorithms.end()};
    }
    
    if(benchmark_str != "")
    {
        unsigned int total_amount_scenarios = 0;
        std::vector<ScenarioLoader> scen_files;
        benchmark_dir = benchmark_str;
        for(auto& p : std::filesystem::recursive_directory_iterator(benchmark_dir))
        {
            if(p.path().extension() == ".scen")
            {
                scen_files.emplace_back(p.path().c_str());
                total_amount_scenarios += scen_files.back().GetNumExperiments();
            }
        }

        std::random_device rd;
        std::mt19937 gen(rd());
        std::discrete_distribution<> dist{(double)total_amount_scenarios, (double)benchmark_amount};

        if(benchmark_amount == 0)
            scenarios.reserve(total_amount_scenarios);
        else
            scenarios.reserve(benchmark_amount);

        while(true)
        {
            for(auto& loader : scen_files)
            {
                for(int i = 0; i < loader.GetNumExperiments(); ++i)
                {
                    if(benchmark_amount == 0)
                    {
                        load_scenario(loader.GetNthExperiment(i), i);
                    }
                    else
                    {
                        if(!dist(gen))
                            continue;

                        load_scenario(loader.GetNthExperiment(i), i);

                        if(scenarios.size() == benchmark_amount)
                            goto benchmark;
                    }
                }
            }
            if(benchmark_amount == 0)
                goto benchmark;
        }

benchmark:
        Benchmarker::benchmark();
        return 0;
    }

    return session.run();
}