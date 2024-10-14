#include <Base/memory/Allocator.h>
#include <Base/memory/MemoryBlocks.h>
#include <Base/log.h>

#include <mutex>

namespace aer::mem {

#if( ALLOCATOR_VERSION == 1)

Allocator::Allocator()
{
    AE_INFO_IF( memoryTracking & MEMORY_TRACKING_REPORT_ACTIONS, "Allocator::Allocator() - Allocator created." );

    _memoryBlocks.resize( ALLOCATOR_AFFINITY_LAST );
    _memoryBlocks[ALLOCATOR_AFFINITY_OBJECTS].reset( new MemoryBlocks{ this } );
}

std::unique_ptr<Allocator>& Allocator::instance() noexcept
{
    static auto allocator = std::make_unique<Allocator>(); 
    return allocator;
}

void* Allocator::allocate( std::size_t size, AllocatorAffinity affinity )
{
    std::scoped_lock lock( _mutex );
    switch ( policy )
    {
        case ALLOCATOR_POLICY_STD_NEW_DELETE:     return operator new( size );
        case ALLOCATOR_POLICY_STD_MALLOC_FREE:    return std::malloc(  size );
        default:                                  break;
    }

    if( affinity > _memoryBlocks.size() )
    {
        AE_INFO_IF( memoryTracking & MEMORY_TRACKING_REPORT_ACTIONS, "Allocator::allocate( %zu, %hhu ) - Creating new memory_block.", size, affinity );
        _memoryBlocks.resize( affinity + 1 );
        _memoryBlocks[affinity].reset( new MemoryBlocks{ this } );
    }

    auto& memoryBlocks = _memoryBlocks[affinity];
    if( memoryBlocks )
    {
        auto ptr = memoryBlocks->allocate( size );
        if( ptr )
        {
            AE_INFO_IF( memoryTracking & MEMORY_TRACKING_REPORT_ACTIONS, "Allocator::allocate( %zu, %hhu ) - Allocated from latest memory_block.", size, affinity );
            return ptr;
        }
    }

    AE_FATAL_IF( memoryBlocks->_blocks.empty(), "Allocator::allocate( %zu, %hhu ) - No memory_block available.", size, affinity );
    return nullptr;
}

bool Allocator::deallocate( void* ptr, std::size_t size )
{
    std::scoped_lock lock( _mutex );
    switch ( policy )
    {
        case ALLOCATOR_POLICY_NO_DELETE:                                return true;
        case ALLOCATOR_POLICY_STD_NEW_DELETE:   operator delete( ptr ); return true;
        case ALLOCATOR_POLICY_STD_MALLOC_FREE:  std::free(       ptr ); return true;
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

#elif( ALLOCATOR_VERSION == 2 )

template<> allocator_new_delete_t::allocator2()
{
    AE_INFO_IF( memoryTracking & MEMORY_TRACKING_REPORT_ACTIONS, "Allocator created with policy NEW_DELETE" );
}

template<> allocator_aer_alloc_t::allocator2()
{
    AE_INFO_IF( memoryTracking & MEMORY_TRACKING_REPORT_ACTIONS, "Allocator created with policy ALLOC_DEALLOC" );
}

template<> allocator_aer_acquire_t::allocator2()
{
    AE_INFO_IF( memoryTracking & MEMORY_TRACKING_REPORT_ACTIONS, "Allocator created with policy ACQUIRE_RETIRE" );
}

template<> void* allocator_new_delete_t::allocate( size_t size )
{
    return operator new( size );
}

template<> bool allocator_new_delete_t::deallocate( void* ptr, size_t size )
{
    operator delete( ptr );
    return true;
}

template<> void* allocator_aer_alloc_t::allocate( size_t size )
{
    return alloc( size );
}

template<> bool allocator_aer_alloc_t::deallocate( void* ptr, size_t size )
{
    dealloc( ptr, size );
    return true;
}

#endif // ALLOCATOR_VERSION == 2

} // namespace aer::mem
