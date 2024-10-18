#include <catch2/catch_test_macros.hpp>

#include <Base/object.h>

namespace aer {

struct Bar : public Object, public Visitor
{
    Bar() = default;
    ~Bar() = default;
    std::string msg{"none"};
};
TYPE_NAME( aer::Bar );

struct Foo : public Object, public Visitor
{
    Foo() = default;
    ~Foo() = default;
    int x{ 42 };
    template< std::derived_from<Bar> T >
    void apply( T& t ) { t.msg = "none"; }
};
TYPE_NAME( aer::Foo );

struct Baz : public Bar
{
    Baz() = default;
    ~Baz() = default;

    template< std::derived_from<Object> T >
    void apply( T& t ) { msg = t.type_name(); }
};
TYPE_NAME( aer::Baz );

} //* namespace aer
namespace aer::test {

TEST_CASE("Runtime type info", "[Object]") {
    Object obj;
    REQUIRE(obj.type_info() == typeid(Object));

    Foo foo;
    REQUIRE(foo.type_info() != typeid(Object));
}

TEST_CASE("type_size", "[Object]") {
    Object obj;
    REQUIRE(obj.type_size() == sizeof(Object));

    Foo foo;
    REQUIRE(foo.type_size() == sizeof(int));
}

TEST_CASE("type_name", "[Object]") {
    Object obj;
    REQUIRE(obj.type_name() == std::string("aer::Object"));

    const Foo foo;
    REQUIRE(foo.type_name() == std::string("const aer::Foo"));

    const Baz baz;
    REQUIRE(baz.type_name() == std::string("aer::Baz"));
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

} //* namespace aer::test