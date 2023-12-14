#include <catch2/catch_test_macros.hpp>

#include "../inc/Base/memory/object.h"
#include "../inc/Base/memory/ref_ptr.h"

TEST_CASE("aer::Object", "[aer::Object]") {
    using namespace aer;
    
    struct Derived : Object {};
    Derived obj;

    SECTION("aer::Object ref_count() returns initial reference count")
    {
        REQUIRE(obj.ref_count() == 0);
    }

    SECTION("aer::Object returns correct reference counts")
    {
        ref_ptr<Derived> ref1, ref2;

        SECTION("aer::Object ref_count() returns correct reference count after _ref()") {
            ref1 = &obj;
            REQUIRE(obj.ref_count() == 1);

            ref2 = &obj;
            REQUIRE(obj.ref_count() == 2);
        }

        SECTION("aer::Object _unref() decreases reference count and deletes when zero") {
            delete ref1;
            REQUIRE(obj.ref_count() == 1);

            delete ref2;
            REQUIRE(obj.ref_count() == 0);
            // The object should be deleted after this _unref() call
        }
    }

    SECTION("aer::Object cast() returns correct pointer type") {
        aer::Object* basePtr = &obj;
        Derived* derivedPtr = basePtr->cast<Derived>();

        REQUIRE(derivedPtr == &obj);
    }

    SECTION("aer::Object const cast() returns correct pointer type") {
        const aer::Object* basePtr = &obj;
        const Derived* derivedPtr = basePtr->cast<Derived>();

        REQUIRE(derivedPtr == &obj);
    }
}