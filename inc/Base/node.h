#pragma once

#include "object.h"
#include "memory/Allocator.h"

namespace aer
{

class Node : public Object
{
public:
    Node() = default;
public:
    static void* operator new( size_t size )
    {
        return mem::alloc( size, mem::ALLOCATOR_AFFINITY_NODES );
    }

    template< typename Self > requires std::derived_from< Self, Node >
    void traverse( this Self& self, Visitor& visitor ) { self._traverse( visitor ); }
    
protected:
    void _traverse( Visitor& visitor ) {}
    ~Node() = default;
};

} // namespace aer