#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_floating_point.hpp>
#include <utility>

#include "state.hpp"
#include "algorithms/util.hpp"

using namespace Util;

TEST_CASE("Map data usage", "[map]")
{
    State s = {
        .map = {Node::WALL, Node::UNVISITED, Node::END,
                Node::UNVISITED, Node::UNVISITED, Node::WALL,
                Node::UNVISITED, Node::START, Node::UNVISITED},
        .width = 3,
        .height = 3
    };

    SECTION("Neighbours are gotten")
    {
        std::pair<node_index, dir_t> buffer[8];
        int amount_neighbours = Util::get_neighbours(buffer, s, 1, 1);
        REQUIRE(amount_neighbours == 4);
        REQUIRE(buffer[0].first == 1);
        REQUIRE(buffer[1].first == 7);
        REQUIRE(buffer[2].first == 6);
        REQUIRE(buffer[3].first == 3);

        amount_neighbours = Util::get_neighbours(buffer, s, 1, 0);
        REQUIRE(amount_neighbours == 2);
        REQUIRE(buffer[0] == std::pair<node_index, dir_t>{2, DIR_EAST});
        REQUIRE(buffer[1] == std::pair<node_index, dir_t>{4, DIR_SOUTH});

        amount_neighbours = Util::get_neighbours(buffer, s, 2, 2);
        REQUIRE(amount_neighbours == 1);
        REQUIRE(buffer[0].first == 7);
        REQUIRE(buffer[0].second == DIR_WEST);
    }

    SECTION("Legal & illegal moves detected")
    {
        REQUIRE(Util::is_move_valid(s, 1, 0, DIR_SOUTH));
        REQUIRE_FALSE(Util::is_move_valid(s, 0, 1, DIR_NORTH));
        REQUIRE(Util::is_move_valid(s, 0, 2, DIR_NORTHEAST));
        REQUIRE_FALSE(Util::is_move_valid(s, 2, 0, DIR_SOUTHWEST));
        REQUIRE_FALSE(Util::is_move_valid(s, 0, 2, DIR_SOUTH));
        REQUIRE_FALSE(Util::is_move_valid(s, 0, 2, DIR_WEST));
        REQUIRE_FALSE(Util::is_move_valid(s, 0, 2, DIR_SOUTHWEST));
        REQUIRE_FALSE(Util::is_move_valid(s, 0, 2, DIR_SOUTHEAST));
        REQUIRE_FALSE(Util::is_move_valid(s, 0, 1, DIR_NORTHEAST));
    }

    SECTION("(x, y) <-> node_index conversion")
    {
        node_index idx = 7;
        auto [x, y] = Util::expand(s.width, idx);
        REQUIRE(x == 1);
        REQUIRE(y == 2);
        REQUIRE(Util::flatten(s.width, x, y) == 7);
    }

    SECTION("node status (empty, wall, etc.) checks")
    {
        REQUIRE(is_wall(s, 0, 0));
        REQUIRE_FALSE(is_wall(s, 4, 4));
        REQUIRE_FALSE(is_wall(s, 1, 1));
        REQUIRE(is_empty(s, 1, 1));
        REQUIRE_FALSE(is_wall(s, 1, 1));
        REQUIRE_FALSE(is_empty(s, -1, -1));
    }

    SECTION("path building")
    {
        s.begin = {1, 2};
        s.end = {2, 0};
        struct MockNode
        {
            node_index prev = NULL_NODE_IDX;
        };
        MockNode path[] =
        {
            {}, {4}, {1}, {}, {7}, {}, {}, {}, {}
        };
        Algorithm::Result mockres = {};
        build_path(s, path, mockres);
        REQUIRE(mockres.path.size() == 3);
        REQUIRE(mockres.path[0] == Point{1, 1});
        REQUIRE(mockres.path[1] == Point{1, 0});
        REQUIRE(mockres.path[2] == Point{2, 0});
        REQUIRE(s.map[2] == Node::END);
        REQUIRE(s.map[7] == Node::START);
    }
}

TEST_CASE("heuristic functions", "[maths]")
{
    auto sqrt_2_heuristic = euclidian_distance(0, 0, 1, 1);
    REQUIRE_THAT(sqrt_2_heuristic, Catch::Matchers::WithinAbs(std::sqrt(2), 0.0001));

    auto diagonal_heuristic = diagonal_distance(0, 0, 3, 3);
    REQUIRE_THAT(diagonal_heuristic, Catch::Matchers::WithinAbs(3 * std::sqrt(2), 0.0001));
}