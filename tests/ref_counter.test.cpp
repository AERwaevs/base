 #include <catch2/catch_test_macros.hpp>

#include <Base/memory/ref_counter.h>

TEST_CASE("Increment and decrement returns appropriate flags", "[ref_counter]") {
    using namespace aer::mem;
    ref32_t counter;
    REQUIRE(counter.load() == 1);

    SECTION("Increment & Decrement") {
        REQUIRE(counter.increment() == true);
        REQUIRE(counter.load() == 2);

        REQUIRE(counter.decrement() == false);
        REQUIRE(counter.load() == 1);
    }

    SECTION("Decrement to zero is flagged") {
        REQUIRE(counter.decrement() == true);
        REQUIRE(counter.load() == 0);
    }

    SECTION("Increment from zero fails") {
        counter.reset(0);
        REQUIRE(counter.load() == 0);

        REQUIRE(counter.increment() == false);
        REQUIRE(counter.load() == 0);
    }
}

#include <thread>
#include <functional>
#include <chrono>

void sleep() {
    std::this_thread::sleep_for(std::chrono::nanoseconds(1));
}

TEST_CASE("Atomic Reference Counter Thread Safety Tests", "[ref_counter][thread_safety]") {
    using namespace aer::mem;
    using task = std::function<void()>;
    ref32_t counter;
    
    const int num_threads           = std::thread::hardware_concurrency();
    const int increments_per_thread = 1000;
    const int decrements_per_thread = 999;
    const int total_increments      = num_threads * increments_per_thread;
    const int total_decrements      = num_threads * decrements_per_thread;
    const int total_actions         = total_increments + total_decrements;
    
    const task increment_task = [&]{ for( int i = 0; i < increments_per_thread; i++ ) counter.increment(); };
    const task decrement_task = [&]{ for( int i = 0; i < decrements_per_thread; i++ ) counter.decrement(); };

    SECTION("Concurrent Increments"){
        std::vector<std::thread> threads;
        for (int i = 0; i < num_threads; i++) {
            threads.emplace_back(increment_task);
        }

        for (auto& thread : threads) {
            thread.join();
        }

        REQUIRE(counter.load() == 1 + total_increments);
    }

    SECTION("Concurrent Increment and Decrement") {

        std::vector<std::thread> threads;
        for (int i = 0; i < num_threads; i++) {
            threads.emplace_back(i % 2 == 0 ? increment_task : decrement_task);
        }

        for (auto& thread : threads) {
            thread.join();
        }

        REQUIRE(counter.load() == 1);
    }
}