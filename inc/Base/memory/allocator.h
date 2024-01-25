#pragma once

#include <memory>

#include "../log.h"

#include "MemorySlots.h"

namespace aer
{
namespace mem
{

enum AllocatorType : uint8_t
{
    ALLOCATOR_TYPE_NO_DELETE = 0,
    ALLOCATOR_TYPE_STD_NEW_DELETE,
    ALLOCATOR_TYPE_STD_MALLOC_FREE,
    ALLOCATOR_TYPE_AER_ALLOC_DEALLOC,
    ALLOCATOR_TYPE_AER_ACQUIRE_RETIRE
};

enum AllocatorAffinity : uint8_t
{
    ALLOCATOR_AFFINITY_OBJECTS  = 0,
    ALLOCATOR_AFFINITY_LAST     = ALLOCATOR_AFFINITY_OBJECTS + 1
};

class Allocator
{
    struct MemoryBlock;
    struct MemoryBlocks;
    mutable std::mutex _mutex;
public:
    AllocatorType   allocatorType      = ALLOCATOR_TYPE_AER_ALLOC_DEALLOC;
    AllocatorType   blockAllocatorType = ALLOCATOR_TYPE_STD_NEW_DELETE;
    MemoryTracking  memoryTracking     = MEMORY_TRACKING_DEFAULT;

    static std::unique_ptr<Allocator>& instance() noexcept
    {
        static std::unique_ptr<Allocator> allocator( new Allocator() ); return allocator;
    }

    void* allocate( size_t size, AllocatorAffinity affinity = ALLOCATOR_AFFINITY_OBJECTS )
    {
        std::scoped_lock lock( _mutex );
        switch ( allocatorType )
        {
            case ALLOCATOR_TYPE_STD_NEW_DELETE:     return operator new( size ); break;
            case ALLOCATOR_TYPE_STD_MALLOC_FREE:    return std::malloc(  size ); break;
            default:                                break;
        }
    
        if( affinity > _memoryBlocks.size() )
        {
            AE_INFO_IF( memoryTracking & MEMORY_TRACKING_REPORT_ACTIONS, "Allocator::allocate( %zu, %hhu ) - Creating new MemoryBlock.", size, affinity );
            _memoryBlocks.resize( affinity + 1 );
            _memoryBlocks[affinity].reset( new MemoryBlocks{ this, 1024 * 1024 } );
        }

        auto& memoryBlocks = _memoryBlocks[affinity];
        if( memoryBlocks->latestBlock )
        {
            auto ptr = memoryBlocks->allocate( size );
        }

        return nullptr;
    }

    bool deallocate( void* ptr, std::size_t size )
    {
        std::scoped_lock lock( _mutex );
        switch ( allocatorType )
        {
            case ALLOCATOR_TYPE_NO_DELETE:                                  return true;
            case ALLOCATOR_TYPE_STD_NEW_DELETE:     operator delete( ptr ); return true;
            case ALLOCATOR_TYPE_STD_MALLOC_FREE:    std::free(       ptr ); return true;
            default: break;
        }

        for( auto& memoryBlocks : _memoryBlocks )
        {
            if( memoryBlocks && memoryBlocks->deallocate( ptr, size ) )
            AE_INFO_IF( memoryTracking & MEMORY_TRACKING_REPORT_ACTIONS, "Allocator::deallocate( %p, %zu ) - Deallocated from memory block", ptr, size );
            return true;
        }
    
        return false;
    }
private:
    struct MemoryBlock
    {
        friend Allocator;
        friend MemoryBlocks;
        
        MemoryBlock( size_t in_size, AllocatorType in_type, MemoryTracking memoryTracking = MEMORY_TRACKING_DEFAULT )
            : type( in_type ), _slots( in_size, memoryTracking )
        {
            switch ( type )
            {
                case ALLOCATOR_TYPE_STD_NEW_DELETE:     _memory = static_cast<uint8_t*>( operator new( in_size ) ); break;
                case ALLOCATOR_TYPE_STD_MALLOC_FREE:    _memory = static_cast<uint8_t*>( std::malloc(  in_size ) ); break;
                default:                                _memory = static_cast<uint8_t*>( operator new( in_size ) ); break;
            }

            AE_INFO_IF( memoryTracking & MEMORY_TRACKING_REPORT_ACTIONS, "Allocator::MemoryBlock::MemoryBlock() - %zu bytes allocated.", in_size );
        }

        ~MemoryBlock()
        {
            switch ( type )
            {
                case ALLOCATOR_TYPE_NO_DELETE:                                      break;
                case ALLOCATOR_TYPE_STD_NEW_DELETE:     operator delete( _memory ); break;
                case ALLOCATOR_TYPE_STD_MALLOC_FREE:    std::free(       _memory ); break;
                default:                                operator delete( _memory ); break;
            }

            AE_INFO_IF( _slots.memoryTracking & MEMORY_TRACKING_REPORT_ACTIONS, "Allocator::MemoryBlock::MemoryBlock~() - %zu bytes deallocated.", _slots.totalMemorySize() );
        }

        void* allocate( size_t size )
        {
            auto offset = _slots.reserve( size );
            return offset.has_value() ? _memory + offset.value() : nullptr;
        }

        bool deallocate( void* ptr, size_t size )
        {
            if( ptr >= _memory )
            {
                size_t offset = static_cast<uint8_t*>( ptr ) - _memory;
                if( offset < _slots.totalMemorySize() )
                {
                    if( !_slots.release( offset, size ) )
                    {
                        AE_WARN_IF( _slots.memoryTracking & MEMORY_TRACKING_REPORT_ACTIONS, "Allocator::MemoryBlock::deallocate() - %zu bytes at offset %zu could not be released.", size, offset );
                    }
                    return true;
                }
            }
            return false;
        }
        
        const AllocatorType type;
    protected:
        MemorySlots _slots;
        uint8_t*    _memory = nullptr;
    };

    struct MemoryBlocks
    {
        MemoryBlocks( Allocator* in_parent, size_t in_blockSize )
            : parent( in_parent ), blockSize( in_blockSize )
        {
            AE_INFO_IF( parent->memoryTracking & MEMORY_TRACKING_REPORT_ACTIONS, "Allocator::MemoryBlocks::MemoryBlocks( %p, %zu ).", parent, blockSize );
        }

        ~MemoryBlocks()
        {
            AE_INFO_IF( parent->memoryTracking & MEMORY_TRACKING_REPORT_ACTIONS, "Allocator::MemoryBlocks::~MemoryBlocks( %p, %zu ).", parent, blockSize );
        }

        void* allocate( size_t size )
        {
            if( latestBlock )
            {
                auto ptr = latestBlock->allocate( size );
                if( ptr ) return ptr;
            }

            for( auto itr = blocks.rbegin(); itr != blocks.rend(); ++itr )
            {
                auto& block = itr->second;
                if( block != latestBlock )
                {
                    auto ptr = block->allocate( size );
                    if( ptr ) return ptr;
                }
            }

            auto block = std::make_shared<MemoryBlock>( std::max( size, blockSize ), parent->blockAllocatorType, parent->memoryTracking );
            latestBlock = block;
            auto ptr = block->allocate( size );

            blocks[block->_memory] = std::move( block );
            AE_INFO_IF( parent->memoryTracking & MEMORY_TRACKING_REPORT_ACTIONS, "Allocator::MemoryBlocks::allocate( %zu ) - allocating in new MemoryBlock.", size );
            return ptr;
        }

        bool deallocate( void* ptr, size_t size )
        {
            if( blocks.empty() ) return false;
            
            auto itr = blocks.upper_bound( ptr );
            if( itr != blocks.end() )
            {
                if( itr != blocks.begin() ) itr--;

                auto& block = itr->second;
                if( block->deallocate( ptr, size ) ) return true;
            }
            else
            {
                auto& block = blocks.rbegin()->second;
                if( block->deallocate( ptr, size ) ) return true;
            }

            AE_WARN_IF( parent->memoryTracking & MEMORY_TRACKING_REPORT_ACTIONS, "Allocator::MemoryBlocks::deallocate( %p, %zu ) - could not find pointer to deallocate.", ptr, size );
            return false;
        }

        Allocator*          parent;
        const size_t        blockSize;

        std::map<void*, std::shared_ptr<MemoryBlock>> blocks;
        std::shared_ptr<MemoryBlock>                  latestBlock;
    };

    std::vector<std::unique_ptr<MemoryBlocks>> _memoryBlocks;
};

static inline void* alloc( size_t size, AllocatorAffinity affinity = ALLOCATOR_AFFINITY_OBJECTS )
{
    return Allocator::instance()->allocate( size, affinity );
}

static inline void dealloc( void* ptr, size_t size = 0 )
{
    Allocator::instance()->deallocate( ptr, size );
}

} // namespace aer::mem
} // namespace aer