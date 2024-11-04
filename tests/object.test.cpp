#include <Base/object.h>
using namespace aer;

struct Bar : public Object
{
    Bar() = default;
    ~Bar() = default;
    std::string msg{"none"};

    template< std::derived_from<Object> T >
    void accept( T& t ) { msg = t.type_name(); }
};
EXT_TYPE_NAME( Bar );

struct Foo : public Object, public Visitor
{
    Foo() = default;
    ~Foo() = default;
    size_t x{ 42 };
    template< std::derived_from<Bar> T >
    void apply( T& t ) { t.msg = type_name( std::declval( this ) ); }
};
EXT_TYPE_NAME( Foo );

struct Baz : public Bar, public Visitor
{
    Baz() = default;
    ~Baz() = default;

    template< std::derived_from<Object> T >
    void apply( this auto&& self, T&& t ) { self.msg = t.type_name(); }

    template< std::derived_from<Foo> T >
    void apply( this auto&& self, T&& t ) { t.x = self.type_info().hash_code(); }
};
EXT_TYPE_NAME( Baz );

const auto z = mem::ref32_t::ZERO_FLAG;
const auto p = mem::ref32_t::ZERO_PENDING_FLAG;
const auto b = z > p;

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
        CHECK_THAT(obj.type_name(), ContainsSubstring("Object"));

        const Foo foo;
        CHECK_THAT(aer::type_name( foo ), ContainsSubstring("Foo"));

        const Baz baz;
        CHECK_THAT(baz.type_name(), ContainsSubstring("Baz"));
    }
}
#include <list>

TEST_CASE("accept", "[Object]") {
    Object obj;
    Foo foo;
    Bar bar;
    Baz baz;

    std::list<Object*> objects{ &obj, &foo, &bar, &baz };

    SECTION("accept", "[Object]") {
        for( auto& object : objects ) {
            object->accept( foo );
        }

        REQUIRE( bar.msg == "Foo" );
        REQUIRE( baz.msg == "Baz" );
        REQUIRE( foo.x == foo.type_info().hash_code() );
    }
}