#pragma once

#include "../object.h"
#include "../memory/Allocator.h"

namespace aer {

struct Node : public Object
{
    Node()  = default;
    ~Node() = default;
    
    static void* operator new( size_t count ) { return mem::alloc( count, mem::ALLOCATOR_AFFINITY_NODES ); }
    static void  operator delete( void* ptr ) { mem::dealloc( ptr, mem::ALLOCATOR_AFFINITY_NODES ); }
    
    constexpr void traverse( Visitor& v ) noexcept {}

    template< typename Self, std::derived_from<Visitor> V > constexpr
    void traverse( this Self&& self, V& visitor ) {};
};

} // namespace aer