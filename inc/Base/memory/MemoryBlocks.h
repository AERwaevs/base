#pragma once

#include "MemoryBlock.h"

namespace aer::mem
{

class Allocator;
// controls a collection of MemoryBlock instances
struct MemoryBlocks
{
    friend Allocator;

    MemoryBlocks( Allocator* in_parent, size_t in_blockSize = MemoryBlock::DEFAULT_BLOCK_SIZE );
    ~MemoryBlocks();
    
    Allocator const*    parent;
    const size_t        blockSize;

protected:
    void* allocate( size_t );
    bool deallocate( void*, size_t );

    std::map<void*, std::shared_ptr<MemoryBlock>> _blocks;
    std::shared_ptr<MemoryBlock>                  _latestBlock;
};

} // namespace aer::mem
