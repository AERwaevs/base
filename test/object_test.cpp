#include <catch2/catch_all.hpp>

#include "../inc/Base/object.h"
#include "../inc/Base/inherit.h"
#include "../inc/Base/memory/ref_ptr.h"
namespace aer
{
    struct Foo : Inherit< Foo, Object >
    {
        int x;
        
        Foo( Foo& ) = default;

        Foo( int in_x = 42 ) : x( in_x ) {}
    };
    AER_TYPE_NAME( aer::Foo );

    struct Bar : Inherit< Bar, Foo >
    {
        std::string y = "y";

        Bar() = default;
        Bar( Bar& ) = default;

        Bar( std::string in_y ) : y( in_y ) {}
        Bar( int in_x, std::string in_y ) : y( in_y ), Inherit( in_x ) {}
    };
    AER_TYPE_NAME( aer::Bar );

    struct Baz : Inherit< Baz, Foo >
    {
        ref_ptr<Bar> bar;
        Baz( ref_ptr<Foo> in_foo, ref_ptr<Bar> in_bar ) : Inherit( in_foo->x ), bar(in_bar) {}
    };
};

TEST_CASE("aer::Object", "[Object]") {
    using namespace aer;

    ref_ptr<Foo> foo;
    ref_ptr<Bar> bar;
    ref_ptr<Baz> baz;

    SECTION("Creation")
    {
        foo = Foo::create_if( false );
        REQUIRE( foo.valid() == false );
    
        foo = Foo::create_if( true );
        REQUIRE( foo.valid() == true );
        REQUIRE( foo->ref_count() == 1 );

        bar = Bar::create( 59, "hello" );
        REQUIRE( bar->x == 59 );
        REQUIRE( bar->y == "hello" );

        baz = Baz::create( foo, bar );
        REQUIRE( baz->x == 64 );
        REQUIRE( baz->bar->y == "hello" );

        baz->bar->y = "world";
        REQUIRE( bar->y == "world" );
    }

    SECTION("type_info()")
    {
        REQUIRE( foo->type_info() != typeid(Bar) );
        REQUIRE( foo->type_info() == typeid(Foo) );
        REQUIRE( bar->type_info() != typeid(Foo) );
    }

    SECTION("is_compatible()")
    {
        // Types are not forward compatible (parent -> child)
        REQUIRE( !foo->is_compatible( bar->type_info() ) );
        // Types are backwards compatible (child -> parent)
        REQUIRE( bar->is_compatible( foo->type_info() ) );
    }

    SECTION("type_name()")
    {
        REQUIRE( std::string("aer::Foo").compare( foo->type_name() ) );
        REQUIRE( std::string("aer::Bar").compare( bar->type_name() ) );
        REQUIRE( std::string("aer::Baz").compare( baz->type_name() ) );
    }

    SECTION("type_size()")
    {
        REQUIRE( foo->type_size() == sizeof(Foo) );
        REQUIRE( bar->type_size() == sizeof(Bar) );
    }
}