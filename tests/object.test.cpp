#include <Base/object.h>
using namespace aer;

struct Bar : public Object, public Visitor
{
    Bar() = default;
    ~Bar() = default;
    std::string msg{"none"};
};
EXT_TYPE_NAME( Bar );

struct Foo : public Object, public Visitor
{
    Foo() = default;
    ~Foo() = default;
    uint32_t x{ 42 };
    template< std::derived_from<Bar> T >
    void apply( T& t ) { t.msg = "none"; }
};
EXT_TYPE_NAME( Foo );

struct Baz : public Bar
{
    Baz() = default;
    ~Baz() = default;

    template< std::derived_from<Object> T >
    void apply( T& t ) { msg = t.type_name(); }
};
EXT_TYPE_NAME( Baz );

#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_string.hpp>

TEST_CASE("Runtime type info", "[Object]") {

    SECTION( "type_info", "[Object]" ) {
        Object obj;
        Foo foo;

        REQUIRE( obj.type_info() == typeid(Object) );
        REQUIRE( foo.type_info() != typeid(Object) );
        REQUIRE( foo.type_info() == typeid(Foo) );
    }
    
    SECTION("type_size", "[Object]") {
        Object obj;
        REQUIRE(obj.type_size() == sizeof(Object));

        Foo foo;
        REQUIRE(foo.type_size() == sizeof(Foo));
    }

    SECTION("type_name", "[Object]") {
        using Catch::Matchers::ContainsSubstring;
        using Catch::Matchers::Equals;

        Object obj;
        REQUIRE_THAT(obj.type_name(), Equals("Object"));

        const Foo foo;
        REQUIRE_THAT(foo.type_name(), Equals("const Foo"));

        const Baz baz;
        CHECK_THAT(baz.type_name(), ContainsSubstring("Baz"));
    }
}

TEST_CASE("Object inheritance compatability", "[Object]"){
    
}

TEST_CASE("accept", "[Object]") {
    Object obj;
    Foo foo;
    Bar bar;
    Baz baz;

    SECTION("Dummy accept calls") {
        obj.accept(bar);
        foo.accept(bar);
        bar.accept(bar);
        baz.accept(bar);
    }

    bar.apply(obj);
    REQUIRE( bar.msg == "none" );
    bar.apply(foo);
    REQUIRE( bar.msg == "none" );
    bar.apply(baz);
    REQUIRE( bar.msg == "none" );

    baz.apply(obj);
    REQUIRE( baz.msg == "aer::Object" );
    baz.apply(foo);
    REQUIRE( baz.msg == "none" );
}