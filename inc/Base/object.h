#pragma once

#include <concepts>
#include <filesystem>
#include <ios>
#include <fstream>

#include "memory/Allocator.h"
#include "memory/ref_counter.h"

#include "memory/ref_ptr.h"

#include "type_name.h"
#include "log.h"

namespace aer
{
    
class Object
{
public:
    static void* operator new( size_t size )  { return mem::alloc( size ); }
    static void  operator delete( void* ptr ) { mem::dealloc( ptr ); }

    template< typename Self > constexpr
    auto& type_info( this Self&& ) noexcept { return typeid( Self ); }

    template< typename Self > constexpr
    auto type_size( this Self&& ) noexcept { return sizeof( Self ); }

    template< typename Self > constexpr
    auto type_name( this Self&& ) noexcept { return aer::type_name< Self >(); }

    template< typename Self > constexpr
    bool is_compatible( this Self&&, const std::type_info& type ) noexcept { return typeid( Self ) == type; }

    template< typename Self, typename V > constexpr
    void accept( this Self&& self, V& visitor ) { visitor.visit( self ); }

    template< typename Self > constexpr
    bool write ( this Self&& self, std::string& path )
    {
        std::ofstream file( path, std::ios::binary );
        AE_WARN_IF( !file.is_open(), "Failed to open file: %s", path.c_str() );
        if( !file.is_open() ) return false;

        file.write( reinterpret_cast<const char*>( self ), sizeof( Self ) );
        file.close();

        return true;
    }

    inline auto ref_count( std::memory_order order = std::memory_order_relaxed ) const noexcept
    { 
        return _references.load(); 
    }
protected:
    template< typename T >
    static inline auto read( const std::string& path )
    {
        std::ifstream file( path, std::ios::ate | std::ios::binary );
        AE_FATAL_IF( !file.is_open(), "Failed to open file: %s", path.c_str() );

        std::size_t         size( file.tellg() );
        std::vector<char>   buffer( size );

        file.seekg( 0 );
        file.read( buffer.data(), size );
        file.close();

        return ref_ptr<T>( new T( buffer ) );
    }
    
    explicit Object() noexcept : _references( 0 ) {}
    virtual ~Object() noexcept       = default;
             Object( Object&& )      = delete;
             Object( const Object& ) = delete;

    inline void _ref( std::memory_order order = std::memory_order_relaxed ) const noexcept
    { 
        _references.increment();
    }

    inline void _unref( std::memory_order order = std::memory_order_seq_cst ) const noexcept 
    { 
        if( _references.decrement() ) delete this;
    }

protected:
    template< typename T >
    friend class ref_ptr;
    
private:
    mutable mem::ref_counter< uint32_t > _references;
};

template< std::derived_from<Object> T, typename... Args >
requires( std::constructible_from<T, Args...> )
constexpr inline ref_ptr<T> create( Args&&... args )
{
    return ref_ptr{ new T(std::forward<Args>( args )...) };
};

} // namespace aer