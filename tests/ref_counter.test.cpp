#pragma once

#include <catch2/catch_test_macros.hpp>

#include <Base/memory/ref_counter.h>

TEST_CASE("Atomic Reference Counter Tests", "[ref_counter]") {
    using namespace aer::mem;

    SECTION("Initial value is 1") {
        ref32_t counter;
        REQUIRE(counter.load() == 1);
    }

    SECTION("Increment and Decrement") {
        ref32_t counter;
        REQUIRE(counter.increment() == true);
        REQUIRE(counter.load() == 2);
        REQUIRE(counter.decrement() == false);
        REQUIRE(counter.load() == 1);
    }

    SECTION("Decrement to zero") {
        ref32_t counter;
        REQUIRE(counter.decrement() == true);
        REQUIRE(counter.load() == 0);
    }

    SECTION("Increment from zero fails") {
        ref32_t counter;
        REQUIRE(counter.decrement() == true);
        REQUIRE(counter.load() == 0);
        REQUIRE(counter.increment() == false);
        REQUIRE(counter.load() == 0);
    }

    SECTION("Reset counter") {
        ref32_t counter;
        counter.reset(5);
        REQUIRE(counter.load() == 5);
    }

    SECTION("Reset counter to zero") {
        ref32_t counter;
        counter.reset(0);
        REQUIRE(counter.load() == 0);
    }
}

#include <thread>
#include <functional>
TEST_CASE("Atomic Reference Counter Thread Safety Tests", "[ref_counter][thread_safety]") {
    using namespace aer::mem;

    SECTION("Concurrent Increment") {
        ref32_t counter;
        const int num_threads = 10;
        const int increments_per_thread = 1000;

        auto increment_task = [&counter, increments_per_thread]() {
            for (int i = 0; i < increments_per_thread; ++i) {
                counter.increment();
            }
        };

        std::vector<std::thread> threads;
        for (int i = 0; i < num_threads; ++i) {
            threads.emplace_back(increment_task);
        }

        for (auto& thread : threads) {
            thread.join();
        }

        REQUIRE(counter.load() == 1 + num_threads * increments_per_thread);
    }

    SECTION("Concurrent Decrement") {
        ref32_t counter;
        counter.reset(10000);
        const int num_threads = 10;
        const int decrements_per_thread = 1000;

        auto decrement_task = [&counter, decrements_per_thread]() {
            for (int i = 0; i < decrements_per_thread; ++i) {
                counter.decrement();
            }
        };

        std::vector<std::thread> threads;
        for (int i = 0; i < num_threads; ++i) {
            threads.emplace_back(decrement_task);
        }

        for (auto& thread : threads) {
            thread.join();
        }

        REQUIRE(counter.load() == 10000 - num_threads * decrements_per_thread);
    }

    SECTION("Concurrent Increment and Decrement") {
        ref32_t counter;
        const int num_threads = 10;
        const int operations_per_thread = 1000;

        std::function<void()> increment_task = [&counter, operations_per_thread]() {
            for (int i = 0; i < operations_per_thread; ++i) {
                counter.increment();
            }
        };

        std::function<void()> decrement_task = [&counter, operations_per_thread]() {
            for (int i = 0; i < operations_per_thread; ++i) {
                counter.decrement();
            }
        };

        std::vector<std::thread> threads;
        for (int i = 0; i < num_threads; ++i) {
            threads.emplace_back(i % 2 == 0 ? increment_task : decrement_task);
        }

        for (auto& thread : threads) {
            thread.join();
        }

        REQUIRE(counter.load() == 1);
    }
}