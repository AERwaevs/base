#include <Base/memory/MemoryBlock.h>
#include <Base/log.h>

namespace aer::mem
{
    
MemoryBlock::MemoryBlock( size_t in_size, AllocatorPolicy in_policy, MemoryTracking memoryTracking )
    : policy( in_policy ), _slots( in_size, memoryTracking )
{
    switch ( policy )
    {
        case ALLOCATOR_POLICY_STD_NEW_DELETE:   _memory = static_cast<uint8_t*>( operator new( in_size ) ); break;
        case ALLOCATOR_POLICY_STD_MALLOC_FREE:  _memory = static_cast<uint8_t*>( std::malloc(  in_size ) ); break;
        default:                                _memory = static_cast<uint8_t*>( operator new( in_size ) ); break;
    }

    AE_INFO_IF( memoryTracking & MEMORY_TRACKING_REPORT_ACTIONS, "Allocator::MemoryBlock::MemoryBlock() - %zu bytes allocated.", in_size );
}

MemoryBlock::~MemoryBlock()
{
    switch ( policy )
    {
        case ALLOCATOR_POLICY_NO_DELETE:                                    break;
        case ALLOCATOR_POLICY_STD_NEW_DELETE:   operator delete( _memory ); break;
        case ALLOCATOR_POLICY_STD_MALLOC_FREE:  std::free(       _memory ); break;
        default:                                operator delete( _memory ); break;
    }

    AE_INFO_IF( _slots.memoryTracking & MEMORY_TRACKING_REPORT_ACTIONS, "Allocator::MemoryBlock::MemoryBlock~() - %zu bytes deallocated.", _slots.totalMemorySize() );
}

void* MemoryBlock::allocate( size_t size )
{
    auto offset = _slots.reserve( size );
    return offset.has_value() ? _memory + offset.value() : nullptr;
}

bool MemoryBlock::deallocate( void* ptr, size_t size )
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

} // namespace aer::mem