#pragma once

#include "MemorySlots.h"
#include "AllocatorPolicy.h"

namespace aer::mem
{

struct MemoryBlock
{   
    friend struct MemoryBlocks;

    constexpr static size_t DEFAULT_BLOCK_SIZE = 1024 * 1024;

    const AllocatorPolicy policy;

    MemoryBlock( size_t, AllocatorPolicy, MemoryTracking = MEMORY_TRACKING_DEFAULT );
    ~MemoryBlock();

protected:
    void* allocate( size_t );
    bool  deallocate( void*, size_t );

    MemorySlots           _slots;
    uint8_t*              _memory = nullptr;
};

template< enum block_policy >
struct memory_block2
{
    constexpr static size_t DEFAULT_BLOCK_SIZE = 1024 * 1024;

protected:
    void* allocate( size_t );
    bool  deallocate( void*, size_t );

    uint8_t*              _memory = nullptr;
};

} // namespace aer::mem
