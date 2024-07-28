#include <queue>
#include <random>
#include <utility>
#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_floating_point.hpp>
#include <catch2/benchmark/catch_benchmark.hpp>

#include "algorithms/bucket_queue.hpp"

TEST_CASE("basic insertion & popping", "[data_structure]")
{
    BucketQueue<char> queue{1.0f, 5, 3};

    queue.push(0.0, 123);
    queue.push(1.1, 5);
    queue.push(0.5, 61);
    queue.push(3.0, 0);

    auto e = queue.pop();
    REQUIRE_THAT(e.value, Catch::Matchers::WithinAbs(0.5, 0.0001));
    REQUIRE(e.object == 61);
    
    e = queue.pop();
    REQUIRE_THAT(e.value, Catch::Matchers::WithinAbs(0.0, 0.0001));
    REQUIRE(e.object == 123);

    e = queue.pop();
    REQUIRE_THAT(e.value, Catch::Matchers::WithinAbs(1.1, 0.0001));
    REQUIRE(e.object == 5);

    e = queue.pop();
    REQUIRE_THAT(e.value, Catch::Matchers::WithinAbs(3.0, .0001));
    REQUIRE(e.object == 0);

    REQUIRE(queue.empty());
    REQUIRE(queue.get_realloc_amount() == 0);
}

TEST_CASE("reallocation", "[data_structure]")
{
    BucketQueue<char> queue{0.5f, 4, 1};

    queue.push(0.0f, 54);
    queue.push(0.5f, 1);
    queue.push(1.5f, 2);
    REQUIRE(queue.get_realloc_amount() == 0);

    queue.push(0.4f, 3);
    REQUIRE(queue.get_realloc_amount() == 1);

    queue.push(1.6f, 4);
    REQUIRE(queue.get_realloc_amount() == 1);

    auto e = queue.pop();
    REQUIRE_THAT(e.value, Catch::Matchers::WithinAbs(0.4, 0.0001));
    REQUIRE(e.object == 3);

    e = queue.pop();
    REQUIRE_THAT(e.value, Catch::Matchers::WithinAbs(0.0, 0.0001));

    queue.push(1.5f, 5);
    queue.push(1.5f, 6);
    REQUIRE(queue.get_realloc_amount() == 2);
    queue.push(1.7f, 7);
    REQUIRE(queue.get_realloc_amount() == 3);

    e = queue.pop();
    REQUIRE_THAT(e.value, Catch::Matchers::WithinAbs(0.5, 0.0001));
    e = queue.pop();
    REQUIRE_THAT(e.value, Catch::Matchers::WithinAbs(1.7, 0.0001));
    REQUIRE(e.object == 7);
}

TEST_CASE("repeated access", "[data_structure]")
{
    std::mt19937 gen{0};
    std::uniform_int_distribution dist{0, 2};
    float possibilities[] = {0.0f, 1.0f, 1.414213562f};
    BucketQueue<unsigned int> queue{0.1, 16, 50};
    queue.push(0.0, 0);

    float curr_val = 0.0f;

    for(int i = 0; i < 5000; ++i)
    {
        auto [val, obj] = queue.top();
        REQUIRE(val > curr_val - 0.11f);
        
        for(int i = 0; i < 5; ++i)
        {
            queue.push(val + possibilities[dist(gen)], obj + dist(gen) - 1);
        }
        curr_val = queue.pop().value;
    }
}

TEST_CASE("benchmark against std::priority_queue", "[!benchmark]")
{
    float possibilities[] = {0.0f, 1.0f, 1.414213562f};
    int amount = 5000;
    float start = 10;
    unsigned int start_val = 0;

    BENCHMARK("std::priority_queue")
    {
        std::mt19937 gen{0};
        std::uniform_int_distribution dist{0, 2};

        std::priority_queue<std::pair<float, unsigned int>> queue;
        queue.emplace(start, start_val);
        for(int i = 0; i < amount; ++i)
        {
            auto [val, obj] = queue.top();
            for(int i = 0; i < 5; ++i)
            {
                queue.emplace(val + possibilities[dist(gen)], obj + dist(gen) - 1);
            }
            queue.pop();
        }
        return queue.pop();
    };

    int realloc = 0;
    BENCHMARK("BucketQueue")
    {
        std::mt19937 gen{0};
        std::uniform_int_distribution dist{0, 2};

        BucketQueue<unsigned int> queue{0.1, 16, 50};
        queue.push(start, start_val);
        for(int i = 0; i < amount; ++i)
        {
            auto [val, obj] = queue.top();
            for(int i = 0; i < 5; ++i)
            {
                queue.push(val + possibilities[dist(gen)], obj + dist(gen) - 1);
            }
            queue.pop();
        }
        realloc = queue.get_realloc_amount();
        return queue.pop();
    };

    INFO("realloc: " << realloc);
}