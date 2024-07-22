#include <catch2/catch_session.hpp>
#include <string>
#include <iostream>
#include <filesystem>
#include <unordered_map>
#include <utility>

#include "main.hpp"
#include "benchmarker.hpp"
#include "hog2/ScenarioLoader.h"
#include "hog2/Map.h"

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
            auto& tile = map.GetTile(x, y).tile1;
            if(tile.type == kGrass || tile.type == kGround)
                nodes.emplace_back(Node::UNVISITED);
            else
                nodes.emplace_back(Node::WALL);
        }
    }
    maps.emplace(name, State{.map = std::move(nodes), .width = width, .height = height});
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

void load_scenarios(const char* path)
{
    ScenarioLoader scenarios{path};
    std::cout << "--- num: " << scenarios.GetNumExperiments() << std::endl;
    for(int i = 0; i < scenarios.GetNumExperiments(); ++i)
    {
        load_scenario(scenarios.GetNthExperiment(i), i);
    }
}

int main(int argc, char** argv)
{
    Catch::Session session;

    std::string benchmark_str;

    using namespace Catch::Clara;
    auto cli = session.cli() |
        Opt(benchmark_str, "benchmark directory")
        ["--benchmarks"]("skip the unit tests, instead read and execute benchmarks from the following directory.");

    session.cli(cli);
    int ret = session.applyCommandLine(argc, argv);
    if(ret != 0)
        return ret;
    
    if(benchmark_str != "")
    {
        benchmark_dir = benchmark_str;
        std::cout << "Loading benchmarks from directory: " << benchmark_dir << std::endl;
        for(auto& p : std::filesystem::recursive_directory_iterator(benchmark_dir))
        {
            if(p.path().extension() == ".scen")
            {
                std::cout << "Found scenarios: " << p.path() << std::endl;
                load_scenarios(p.path().c_str());
            }
        }

        Benchmarker::benchmark();
        return 0;
    }

    return session.run();
}