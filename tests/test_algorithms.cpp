#include <filesystem>

#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_floating_point.hpp>

#include "algorithms/algorithm.hpp"
#include "algorithms/a_star.hpp"
#include "algorithms/jps.hpp"
#include "main.hpp"

TEST_CASE("Amount of expansions and examinations", "[algorithm]")
{
    std::filesystem::path test_map_path;

    if(std::filesystem::exists(std::filesystem::current_path() / "test_map.map"))
        test_map_path = std::filesystem::current_path() / "test_map.map";
    else
        test_map_path = std::filesystem::current_path() / "tests" / "test_map.map";

    load_map("test_map", test_map_path.c_str());

    State& s = maps.at("test_map");
    s.begin = {4, 4};
    s.end = {8, 2};

    SECTION("A*")
    {
        AStar a_star;
        a_star.init(&s);
        a_star.update();

        REQUIRE(a_star.get_result().expanded == 1);
        REQUIRE(a_star.get_result().examined == 7);
        a_star.update();
        REQUIRE(a_star.get_result().expanded == 2);
        REQUIRE(a_star.get_result().examined == 15);

        while(a_star.update() == Algorithm::Result::Type::EXECUTING) {}

        auto p = a_star.get_result().path;
        REQUIRE(p.size() == 6);

        REQUIRE_THAT(a_star.get_result().length, Catch::Matchers::WithinAbs(6.8284, 0.0001));
        REQUIRE(a_star.get_result().expanded == 16);
        REQUIRE(a_star.get_result().examined == 105);
    }

    SECTION("JPS")
    {
        JumpPointSearch jps;
        jps.init(&s);
        jps.update();

        REQUIRE(jps.get_result().expanded == 1);
        REQUIRE(jps.get_result().examined == 44);

        jps.update();
        REQUIRE(jps.get_result().expanded == 2);
        REQUIRE(jps.get_result().examined == (44 + 10));

        jps.update();
        REQUIRE(jps.get_result().expanded == 3);
        REQUIRE(jps.get_result().examined == (44 + 10 + 14));

        REQUIRE(jps.update() == Algorithm::Result::SUCCESS);
        REQUIRE_THAT(jps.get_result().length, Catch::Matchers::WithinAbs(6.8284, 0.0001));
    }
}