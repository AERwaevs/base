#include <Base/memory/MemorySlots.h>

#include <Base/log.h>

namespace aer::mem
{

MemorySlots::MemorySlots( size_t availableMemorySize, MemoryTracking tracking )
    : memoryTracking( tracking ), _totalMemorySize( availableMemorySize )
{
    insert( 0, availableMemorySize );
}

std::optional<offset_t> MemorySlots::reserve( size_t size, size_t alignment )
{
    const auto report = memoryTracking & MEMORY_TRACKING_REPORT_ACTIONS;
    AE_INFO_IF( report, "MemorySlots::reserve( %zu, %zu )", size, alignment );

    if( full() ) return std::nullopt;
    
    auto itr = _availableMemory.lower_bound( size );
    while( itr != _availableMemory.end() )
    {
        size_t   slotSize       = itr->first;
        offset_t slotStart      = itr->second;
        offset_t slotEnd        = slotStart + slotSize;
        offset_t alignedStart   = ( ( slotStart + alignment - 1 ) / alignment ) * alignment;
        offset_t alignedEnd     = alignedStart + size;

        // slot is not big enough, advance to next slot
        if( alignedEnd > slotEnd ) { ++itr; continue; }

        // remove the slot that will be used
        remove( slotStart, slotSize );

        // create slots for either side of the newly reserved memory 
        if( slotStart < alignedStart ) insert( slotStart, alignedStart - slotStart );
        if( alignedEnd < slotEnd )     insert( alignedEnd, slotEnd - alignedEnd );

        // record and return the reserved slot
        _reservedMemory.emplace( alignedStart, size );
        return alignedStart;
    
        AE_INFO_IF( report, "MemorySlots::reserve() - %zu bytes reserved at offset %zu.", size, alignedStart );
    }

    AE_INFO_IF( report, "MemorySlots::reserve() - %zu bytes requested, but no slot was big enough.", size );
    return std::nullopt;
}

bool MemorySlots::release( offset_t offset, size_t size )
{
    const auto report = memoryTracking & MEMORY_TRACKING_REPORT_ACTIONS;
    AE_INFO_IF( report, "MemorySlots::release( %zu )", offset );

    auto itr = _reservedMemory.find( offset );
    if( itr == _reservedMemory.end() ) return false; // entry not found

    if( size != itr->second )
    {
        AE_INFO_IF( report, "MemorySlots::release() - %zu bytes requested, but %zu bytes were reserved.", size, itr->second );
        size = itr->second;
    }

    // remove the reserved slot
    _reservedMemory.erase( itr );
    if( _offsetSizes.empty() )
    {
        insert( offset, size );
        return true;
    }

    auto slotStart = offset;
    auto slotEnd   = offset + size;
    
    // merge with next slot if adjacent
    auto nextItr = _offsetSizes.upper_bound( offset );
    if( nextItr != _offsetSizes.end() && nextItr->first == slotEnd )
    {
        slotEnd = nextItr->first + nextItr->second;
        remove( nextItr->first, nextItr->second );
    }
    
    // merge with previous slot if adjacent
    auto prevItr = nextItr != _offsetSizes.end() ? std::prev( nextItr ) : _offsetSizes.end();
    auto prevSlotEnd = prevItr->first + prevItr->second;
    if( prevSlotEnd == slotStart )
    {
        slotStart = prevItr->first;
        remove( prevItr->first, prevItr->second );
    }

    // insert the merged slot back into the available memory
    insert( slotStart, slotEnd - slotStart );
    return true;
}

void MemorySlots::report() const
{
    LOG_SCOPE_F( WARNING, "MemorySlots::report()" );
    AE_INFO( "Total available memory: %zu bytes.", _totalMemorySize );
    AE_INFO( "Total reserved memory:  %zu bytes.", totalReservedSize() );
    AE_INFO( "Total free memory:      %zu bytes.", totalAvailableSize() );
    AE_INFO( "Total used memory:      %zu bytes.", _totalMemorySize - totalAvailableSize() );

    AE_INFO( "Available memory slots:" );
    for( auto& slot : _availableMemory )
    {
        AE_INFO( "  %zu bytes at offset %zu.", slot.first, slot.second );
    }

    AE_INFO( "Reserved memory slots:" );
    for( auto& slot : _reservedMemory )
    {
        AE_INFO( "  %zu bytes at offset %zu.", slot.second, slot.first );
    }
}

bool MemorySlots::check() const
{
    LOG_SCOPE_F( INFO, "MemorySlots::check()" );

    if( _availableMemory.size() != _offsetSizes.size() )
    {
        AE_ERROR( "Available memory size (%zu) does not match offset sizes (%zu).", _availableMemory.size(), _offsetSizes.size() );
        return false;
    }

    size_t availableSize = std::accumulate( _offsetSizes.begin(), _offsetSizes.end(), 0, []( auto sum, auto& slot ) { return sum + slot.second; } );
    size_t reservedSize  = std::accumulate( _reservedMemory.begin(), _reservedMemory.end(), 0, []( auto sum, auto& slot ) { return sum + slot.second; } );
    size_t computedSize  = availableSize + reservedSize;

    if( computedSize != _totalMemorySize )
    {
        AE_ERROR( "Computed size (%zu) does not match total available size (%zu).", computedSize, _totalMemorySize );
        report();
        return false;
    }

    return true;
}

void MemorySlots::insert( offset_t offset, size_t size )
{
    _offsetSizes.emplace( offset, size );
    _availableMemory.emplace( size, offset );
}

void MemorySlots::remove( offset_t offset, size_t size )
{
    _offsetSizes.erase( offset );
    for( auto itr = _availableMemory.lower_bound( size ); itr != _availableMemory.upper_bound( size ); ++itr )
    {
        if( itr->second == offset ) { _availableMemory.erase( itr ); break; }
    }
}

} // namespace aer::mem
