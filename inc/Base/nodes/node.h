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
    
    template< typename Self, typename Visitor > constexpr
    void traverse( this Self&& self, Visitor& visitor ) {};
};

} // namespace aer