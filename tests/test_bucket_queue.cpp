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
    REQUIRE_THAT(e.value, Catch::Matchers::WithinAbs(0.0, 0.0001));
    REQUIRE(e.object == 123);
    
    e = queue.pop();
    REQUIRE_THAT(e.value, Catch::Matchers::WithinAbs(0.5, 0.0001));
    REQUIRE(e.object == 61);

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
    REQUIRE_THAT(e.value, Catch::Matchers::WithinAbs(0.0, 0.0001));
    REQUIRE(e.object == 54);

    e = queue.pop();
    REQUIRE_THAT(e.value, Catch::Matchers::WithinAbs(0.4, 0.0001));

    queue.push(1.5f, 5);
    queue.push(1.5f, 6);
    REQUIRE(queue.get_realloc_amount() == 2);
    queue.push(1.7f, 7);
    REQUIRE(queue.get_realloc_amount() == 3);

    e = queue.pop();
    REQUIRE_THAT(e.value, Catch::Matchers::WithinAbs(0.5, 0.0001));
    e = queue.pop();
    REQUIRE_THAT(e.value, Catch::Matchers::WithinAbs(1.5, 0.0001));
    REQUIRE(e.object == 2);
}

TEST_CASE("repeated realloc with different buckets", "[data_structure]")
{
    BucketQueue<int> q{1.0f, 5, 1};

    q.push(0.0f, 444);
    q.push(0.0f, 444);
    q.push(1.5f, 333);
    q.update_write();
    q.push(0.0f, 444);
    q.push(0.1f, 444);
    q.push(0.0f, 444);

    for(int i = 0; i < 5; ++i)
    {
        REQUIRE(q.pop().object == 444);
        q.update_write();
    }
    
    REQUIRE(q.pop().object == 333);
    q.update_write();
    
    q.push(3.512, 123);
    REQUIRE(q.pop().object == 123);
    REQUIRE(q.empty());
}

TEST_CASE("repeated access", "[data_structure]")
{
    std::mt19937 gen{0};
    std::uniform_real_distribution dist{0.0f, 2.82842712f};
    BucketQueue<unsigned int> queue{0.1, 30, 25};
    queue.push(1.0, 420);

    float curr_val = 0.0f;

    for(int i = 0; i < 10000; ++i)
    {
        auto [val, obj] = queue.pop();
        REQUIRE(val > curr_val - 0.11f);
        REQUIRE(obj == 420);

        curr_val = val;
        
        for(int i = 0; i < 4; ++i)
        {
            queue.push(val + dist(gen), 420);
        }
        queue.update_write();
    }
}

TEST_CASE("write_curr and read_curr updated", "[data_structure]")
{
    BucketQueue<long long int> q{1.0f, 5, 1};

    q.push(0.4f, 32);
    q.push(4.0f, 5);
    q.push(4.5f, -123);
    q.push(3.1f, 215);

    REQUIRE(q.pop().object == 32);
    REQUIRE(q.pop().object == 215);
    q.push(0.5f, 66);
    q.update_write();

    REQUIRE(q.pop().object == 66);
    REQUIRE(q.pop().object == 5);
    REQUIRE(q.pop().object == -123);
    REQUIRE(q.empty());
}

TEST_CASE("clear", "[data_structure]")
{
    BucketQueue<int> q{1.0f, 5, 1};

    q.push(0.5f, 23);
    q.push(1.5f, 23);
    q.push(2.5f, 23);

    q.pop();
    q.update_write();
    q.clear();

    REQUIRE(q.empty());
    q.push(8.0f, 61);
    q.push(9.0f, 1);
    REQUIRE(q.pop().object == 61);
    REQUIRE(q.pop().object == 1);
    REQUIRE(q.empty());
}

TEST_CASE("benchmark against std::priority_queue", "[!benchmark]")
{
    int amount = 5000;
    float start = 10;
    unsigned int start_val = 0;

    BENCHMARK("std::priority_queue")
    {
        std::mt19937 gen{0};
        std::uniform_real_distribution dist{0.0f, 2.82842712f};

        std::priority_queue<std::pair<float, unsigned int>> queue;
        queue.emplace(start, start_val);
        for(int i = 0; i < amount; ++i)
        {
            auto [val, obj] = queue.top();
            for(int i = 0; i < 5; ++i)
            {
                queue.emplace(val + dist(gen), obj + 1);
            }
            queue.pop();
        }
        return queue.pop();
    };

    int realloc = 0;
    BENCHMARK("BucketQueue")
    {
        std::mt19937 gen{0};
        std::uniform_real_distribution dist{0.0f, 2.82842712f};

        BucketQueue<unsigned int> queue{0.1, 30, 5};
        queue.push(start, start_val);
        for(int i = 0; i < amount; ++i)
        {
            auto [val, obj] = queue.pop();
            for(int i = 0; i < 5; ++i)
            {
                queue.push(val + dist(gen), obj + 1);
            }
            queue.update_write();
        }
        realloc = queue.get_realloc_amount();
        return queue.pop();
    };

    INFO("realloc: " << realloc);
}