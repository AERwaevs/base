#pragma once

#include "object.h"
#include "memory/Allocator.h"

namespace aer
{

class Node : public Object
{
public:
    Node() = default;
    
    static void* operator new( size_t size )
    {
        return mem::alloc( size, mem::ALLOCATOR_AFFINITY_NODES );
    }
protected:
    ~Node() = default;
};

} // namespace aer