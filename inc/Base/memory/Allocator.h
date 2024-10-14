#pragma once

#include <mutex>

#include "MemoryTracking.h"
#include "MemoryBlocks.h"
#include "AllocatorPolicy.h"

#define ALLOCATOR_VERSION 1

namespace aer {
namespace mem {

enum AllocatorAffinity : uint8_t
{
    ALLOCATOR_AFFINITY_OBJECTS  = 0,
    ALLOCATOR_AFFINITY_NODES    = 1,
    ALLOCATOR_AFFINITY_LAST     = ALLOCATOR_AFFINITY_NODES + 1
};

#if( ALLOCATOR_VERSION == 1)

class Allocator
{
public:
    AllocatorPolicy policy          = ALLOCATOR_POLICY_DEFAULT;
    AllocatorPolicy blockPolicy     = ALLOCATOR_POLICY_STD_NEW_DELETE;
    MemoryTracking  memoryTracking  = MEMORY_TRACKING_DEFAULT;

    Allocator();

    static std::unique_ptr<Allocator>& instance() noexcept;

    void* allocate( std::size_t, AllocatorAffinity = ALLOCATOR_AFFINITY_OBJECTS );
    bool  deallocate( void*, std::size_t );
protected:
    std::vector<std::unique_ptr<MemoryBlocks>>  _memoryBlocks;
private:
    mutable     std::mutex                      _mutex;
};

static inline void* alloc( size_t size, AllocatorAffinity affinity = ALLOCATOR_AFFINITY_OBJECTS )
{
    return Allocator::instance()->allocate( size, affinity );
}

static inline void dealloc( void* ptr, size_t size = 0 )
{
    Allocator::instance()->deallocate( ptr, size );
}

#elif( ALLOCATOR_VERSION == 2 )

template< allocator_policy2 policy >
struct allocator2
{
    template< std::derived_from<allocator2> derived >
    static std::unique_ptr<derived>& instance() noexcept;

    allocator2();

    inline void* allocate( size_t, AllocatorAffinity affinity = ALLOCATOR_AFFINITY_OBJECTS );
    inline bool  deallocate( void* ptr, size_t size = 0 );

#ifndef NDEBUG
    MemoryTracking  memoryTracking = MEMORY_TRACKING_DEFAULT;
#endif
};

template<>
struct allocator2<ALLOCATOR_POLICY2_AER_ALLOC_DEALLOC>
{
    std::vector<std::unique_ptr<MemoryBlocks>> _memoryBlocks;
    mutable std::mutex _mutex;
};

using allocator_new_delete_t = allocator2<ALLOCATOR_POLICY2_STD_NEW_DELETE>;
using allocator_aer_alloc_t  = allocator2<ALLOCATOR_POLICY2_AER_ALLOC_DEALLOC>;
using allocator_aer_acquire_t = allocator2<ALLOCATOR_POLICY2_AER_ACQUIRE_RETIRE>;

template<> std::unique_ptr<allocator_new_delete_t>& allocator_new_delete_t::instance() noexcept
{
    static auto allocator = std::make_unique<allocator_new_delete_t>(); 
    return allocator;
}

template<> std::unique_ptr<allocator_aer_alloc_t>& allocator_aer_alloc_t::instance() noexcept
{
    static auto allocator = std::make_unique<allocator_aer_alloc_t>(); 
    return allocator;
}

template<> std::unique_ptr<allocator_aer_acquire_t>& allocator_aer_acquire_t::instance() noexcept
{
    static auto allocator = std::make_unique<allocator_aer_acquire_t>(); 
    return allocator;
}

template< allocator_policy2 policy = ALLOCATOR_POLICY2_DEFAULT >
static inline void* alloc( size_t size, AllocatorAffinity affinity = ALLOCATOR_AFFINITY_OBJECTS )
{
    return allocator2<policy>::instance()->allocate( size, affinity );
}

template< allocator_policy2 policy = ALLOCATOR_POLICY2_DEFAULT >
static inline void dealloc( void* ptr, size_t size = 0 )
{
    allocator2<policy>::instance()->deallocate( ptr, size );
}

#endif // ALLOCATOR_VERSION == 2

} // namespace aer::mem
} // namespace aer