#include <Base/memory/MemoryBlocks.h>
#include <Base/memory/Allocator.h>
#include <Base/log.h>

namespace aer::mem
{

MemoryBlocks::MemoryBlocks( Allocator* in_parent, size_t in_blockSize )
    : parent( in_parent ), blockSize( in_blockSize )
{
    AE_INFO_IF( parent->memoryTracking & MEMORY_TRACKING_REPORT_ACTIONS, "Allocator::MemoryBlocks::MemoryBlocks( %p, %zu ).", parent, blockSize );
}

MemoryBlocks::~MemoryBlocks()
{
    AE_INFO_IF( parent->memoryTracking & MEMORY_TRACKING_REPORT_ACTIONS, "Allocator::MemoryBlocks::~MemoryBlocks( %p, %zu ).", parent, blockSize );
}

void* MemoryBlocks::allocate( size_t size )
{
    if( _latestBlock )
    {
        auto ptr = _latestBlock->allocate( size );
        if( ptr ) return ptr;
    }

    for( auto itr = _blocks.rbegin(); itr != _blocks.rend(); ++itr )
    {
        auto& block = itr->second;
        if( block != _latestBlock )
        {
            auto ptr = block->allocate( size );
            if( ptr ) return ptr;
        }
    }

    auto block = std::make_shared<MemoryBlock>( std::max( size, blockSize ), parent->blockPolicy, parent->memoryTracking );
    _latestBlock = block;
    auto ptr = block->allocate( size );

    _blocks[block->_memory] = std::move( block );
    AE_INFO_IF( parent->memoryTracking & MEMORY_TRACKING_REPORT_ACTIONS, "Allocator::MemoryBlocks::allocate( %zu ) - allocating in new MemoryBlock.", size );
    return ptr;
}

bool MemoryBlocks::deallocate( void* ptr, size_t size )
{
    if( _blocks.empty() ) return false;
    
    auto itr = _blocks.upper_bound( ptr );
    if( itr != _blocks.end() )
    {
        if( itr != _blocks.begin() ) itr--;

        auto& block = itr->second;
        if( block->deallocate( ptr, size ) ) return true;
    }
    else
    {
        auto& block = _blocks.rbegin()->second;
        if( block->deallocate( ptr, size ) ) return true;
    }

    AE_WARN_IF( parent->memoryTracking & MEMORY_TRACKING_REPORT_ACTIONS, "Allocator::MemoryBlocks::deallocate( %p, %zu ) - could not find pointer to deallocate.", ptr, size );
    return false;
}

} // namespace aer::mem