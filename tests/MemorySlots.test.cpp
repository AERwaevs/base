#include <catch2/catch_test_macros.hpp>

#include <Base/memory/MemorySlots.h>

#include <optional>
#include <numeric>

using namespace aer::mem;

TEST_CASE("MemorySlots: Reserve and Release Memory", "[MemorySlots]") {
    MemorySlots memorySlots(1024); // 1 KB of memory
    const auto size = 128;

    SECTION("Reserve memory within available size") {
        auto offset = memorySlots.reserve(size);
        REQUIRE(offset.has_value());
        REQUIRE(*offset == 0);
        REQUIRE(memorySlots.totalReservedSize() == size);
        REQUIRE(memorySlots.totalAvailableSize() == 896);
    }

    SECTION("Reserve memory exceeding available size") {
        auto offset = memorySlots.reserve(2048);
        REQUIRE_FALSE(offset.has_value());
        REQUIRE(memorySlots.totalReservedSize() == 0);
        REQUIRE(memorySlots.totalAvailableSize() == 1024);
    }

    SECTION("Release reserved memory") {
        auto offset = memorySlots.reserve(size);
        REQUIRE(offset.has_value());
        bool released = memorySlots.release(*offset, size);
        REQUIRE(released);
        REQUIRE(memorySlots.totalReservedSize() == 0);
        REQUIRE(memorySlots.totalAvailableSize() == 1024);
    }

    SECTION("Release non-reserved memory") {
        bool released = memorySlots.release(0, size);
        REQUIRE_FALSE(released);
        REQUIRE(memorySlots.totalReservedSize() == 0);
        REQUIRE(memorySlots.totalAvailableSize() == 1024);
    }
}

TEST_CASE("MemorySlots: Full and Empty Checks", "[MemorySlots]") {
    MemorySlots memorySlots(1024); // 1 KB of memory

    SECTION("Initially empty") {
        REQUIRE(memorySlots.empty());
        REQUIRE_FALSE(memorySlots.full());
    }

    SECTION("After reserving all memory") {
        auto offset = memorySlots.reserve(1024);
        REQUIRE(offset.has_value());
        REQUIRE(memorySlots.full());
        REQUIRE_FALSE(memorySlots.empty());
    }

    SECTION("After releasing all memory") {
        auto offset = memorySlots.reserve(1024);
        REQUIRE(offset.has_value());
        memorySlots.release(*offset, 1024);
        REQUIRE(memorySlots.empty());
        REQUIRE_FALSE(memorySlots.full());
    }
}

TEST_CASE("MemorySlots: Data Locality", "[MemorySlots]") {
    MemorySlots memorySlots(1024); // 1 KB of memory
    const auto size = 128;

    SECTION("Reserve and release memory to check data locality") {
        auto offset1 = memorySlots.reserve(size);
        REQUIRE(offset1.has_value());
        REQUIRE(*offset1 == 0);

        auto offset2 = memorySlots.reserve(size);
        REQUIRE(offset2.has_value());
        REQUIRE(*offset2 == size);

        auto numObjects = 2;
        auto offset3 = memorySlots.reserve(size * 2);
        REQUIRE(offset3.has_value());
        REQUIRE(*offset3 == size * numObjects); // Check data locality

        memorySlots.release(*offset1, size);
        auto offset4 = memorySlots.reserve(size);
        REQUIRE(offset4.has_value());
        REQUIRE(*offset4 == *offset1); // Check data locality
    }
}