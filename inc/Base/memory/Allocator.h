#pragma once

#include <mutex>

#include "MemoryTracking.h"
#include "AllocatorPolicy.h"

namespace aer
{
namespace mem
{

enum AllocatorAffinity : uint8_t
{
    ALLOCATOR_AFFINITY_OBJECTS  = 0,
    ALLOCATOR_AFFINITY_NODES    = 1,
    ALLOCATOR_AFFINITY_LAST     = ALLOCATOR_AFFINITY_NODES + 1
};

struct MemoryBlocks;

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

constexpr static inline void* alloc( size_t size, AllocatorAffinity affinity = ALLOCATOR_AFFINITY_OBJECTS )
{
    return Allocator::instance()->allocate( size, affinity );
}

constexpr static inline void dealloc( void* ptr, size_t size = 0 )
{
    Allocator::instance()->deallocate( ptr, size );
}

} // namespace aer::mem
} // namespace aer