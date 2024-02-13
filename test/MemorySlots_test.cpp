#include <catch2/catch.hpp>

#include <Base/memory/MemorySlots.h>

TEST_CASE("aer::MemorySlots", "[aer::MemorySlots]") {
    using namespace aer::mem;

    SECTION("aer::MemorySlots::MemorySlots( size_t, MemoryTracking )") {
        MemorySlots slots( 1024, MEMORY_TRACKING_REPORT_ACTIONS );
    }

    SECTION("aer::MemorySlots::reserve( size_t, size_t )") {
        MemorySlots slots( 1024, MEMORY_TRACKING_REPORT_ACTIONS );

        SECTION("aer::MemorySlots::reserve( size_t, size_t ) returns std::nullopt when full") {
            REQUIRE(slots.reserve( 1024, 1 ) == std::nullopt);
        }

        SECTION("aer::MemorySlots::reserve( size_t, size_t ) returns offset_t when successful") {
            REQUIRE(slots.reserve( 512, 1) == 0);
        }
    }

    SECTION("aer::MemorySlots::release( offset_t, size_t )") {
        MemorySlots slots( 1024, MEMORY_TRACKING_REPORT_ACTIONS );

        SECTION("aer::MemorySlots::release( offset_t, size_t ) returns false when offset not found") {
            REQUIRE(slots.release( 0, 512 ) == false);
        }

        SECTION("aer::MemorySlots::release( offset_t, size_t ) returns true when offset found") {
            slots.reserve( 512, 1);
            REQUIRE(slots.release( 0, 512 ) == true);
        }
    }
}


TEST_CASE("MemorySlots functionality", "[MemorySlots]") {
    SECTION("Initialization") {
        MemorySlots slots( 1024, MEMORY_TRACKING_REPORT_ACTIONS );
        REQUIRE(slots.totalMemorySize()             == 128);
        REQUIRE(slots.totalAvailableSize()          == 128);
        REQUIRE(slots.totalReservedSize()           == 0);
        REQUIRE(slots.empty()                       == true);
        REQUIRE(slots.full()                        == false);
    }
    SECTION("Reserve & Release") {
        MemorySlots slots( 128, MEMORY_TRACKING_REPORT_ACTIONS );
        SECTION("Reserve and release memory") {
            SECTION("aer::MemorySlots::reserve( size_t, size_t )") {
                SECTION("returns offset_t when a slot big enough is available") {
                    REQUIRE(slots.reserve( 7, 8) == 0);
                }
                SECTION("returns std::nullopt when there is no slot big enough") {
                    REQUIRE(slots.reserve( 1024, 1 ) == std::nullopt);
                }
                SECTION("returns correctly aligned offset") {
                    auto offset = slots.reserve(9, 8);
                    REQUIRE(offset.has_value());
                    REQUIRE(*offset % 8 == 0);
                }

                REQUIRE(slots.totalReservedSize() == 24);
                REQUIRE(slots.totalAvailableSize() == 102);
            }
            SECTION("aer::MemorySlots::release( offset_t, size_t )") {
                SECTION("Release memory, but with wrong size") {
                    auto offset = slots.reserve(9, 8);
                    bool released = slots.release(*offset, 64);
                    REQUIRE(released == false);
                    REQUIRE(slots.totalReservedSize() == 24);
                    REQUIRE(slots.totalAvailableSize() == 102);
                }

                SECTION("Release memory, but with an offset that was not reserved") {
                    bool released = slots.release(64, 64);
                    REQUIRE(released == false);
                    REQUIRE(slots.totalReservedSize() == 24);
                    REQUIRE(slots.totalAvailableSize() == 102);
                }

                SECTION("Release memory correctly") {
                    auto offset = slots.reserve(9, 8);
                    bool released = slots.release(*offset, 9);
                    REQUIRE(released == true);
                    REQUIRE(slots.totalAvailableSize() == 120);
                    REQUIRE(slots.totalReservedSize() == 8);
                    REQUIRE(slots.empty() == true);
                    REQUIRE(slots.full() == false);
                }
            }
        }

        SECTION("Reserve more memory than available") {
            auto offset = slots.reserve(256);
            REQUIRE(offset == std::nullopt);
        }

        SECTION("Release unreserved memory") {
            bool released = slots.release(76, 76);
            REQUIRE(released == false);
        }
    }
}