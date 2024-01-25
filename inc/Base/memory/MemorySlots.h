#pragma once

#include <numeric>

namespace aer::mem
{
    
using offset_t = size_t;

/// Hint for what checks/reporting to do when using MemorySlots within Allocator/Buffer/DeviceMemory.
enum MemoryTracking : uint8_t
{
    MEMORY_TRACKING_NO_CHECKS       = 0,
    MEMORY_TRACKING_REPORT_ACTIONS  = 1,
    MEMORY_TRACKING_CHECK_ACTIONS   = 2,
    MEMORY_TRACKING_DEFAULT         = MEMORY_TRACKING_REPORT_ACTIONS
};

class MemorySlots
{
    std::multimap<size_t, offset_t> _availableMemory;
    std::map<offset_t, size_t>      _reservedMemory;
    std::map<offset_t, size_t>      _offsetSizes;
    size_t                          _totalMemorySize;
protected:
    void _insert( offset_t offset, size_t size )
    {
        _offsetSizes.emplace( offset, size );
        _availableMemory.emplace( size, offset );
    }

    void _remove( offset_t offset, size_t size )
    {
        _offsetSizes.erase( offset );
        auto end = _availableMemory.upper_bound( size );
        for( auto itr = _availableMemory.lower_bound( size ); itr != end; ++itr )
        {
            if( itr->second == offset ) { _availableMemory.erase( itr ); break; }
        }
    }
public:
    mutable MemoryTracking memoryTracking;
    
    MemorySlots( size_t availableMemorySize, MemoryTracking tracking = MEMORY_TRACKING_DEFAULT )
        : memoryTracking( tracking ), _totalMemorySize( availableMemorySize )
    {
        _insert( 0, availableMemorySize );
    }

    bool full()  const { return _availableMemory.empty(); }
    bool empty() const { return _offsetSizes.empty(); }

    size_t totalMemorySize() const { return _totalMemorySize; }
    size_t totalAvailableSize() const
    {
        return std::accumulate( _availableMemory.begin(), _availableMemory.end(), 0, []( auto sum, auto& slot ) { return sum + slot.first; } );
    }
    size_t totalReservedSize() const
    {
        return std::accumulate( _reservedMemory.begin(), _reservedMemory.end(), 0, []( auto sum, auto& slot ) { return sum + slot.second; } );
    }

    std::optional<offset_t> reserve( size_t size, size_t alignment = 4 )
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
            _remove( slotStart, slotSize );

            // create slots for either side of the newly reserved memory 
            if( slotStart < alignedStart ) _insert( slotStart, alignedStart - slotStart );
            if( alignedEnd < slotEnd )     _insert( alignedEnd, slotEnd - alignedEnd );

            // record and return the reserved slot
            _reservedMemory.emplace( alignedStart, size );
            return alignedStart;
        
            AE_INFO_IF( report, "MemorySlots::reserve() - %zu bytes reserved at offset %zu.", size, alignedStart );
        }

        AE_INFO_IF( report, "MemorySlots::reserve() - %zu bytes requested, but no slot was big enough.", size );
        return std::nullopt;
    }

    bool release( offset_t offset, size_t size )
    {
        const auto report = memoryTracking & MEMORY_TRACKING_REPORT_ACTIONS;
        AE_INFO_IF( report, "MemorySlots::release( %zu )", offset );

        auto itr = _reservedMemory.find( offset );
        if( itr == _reservedMemory.end() ) return false; // entry not found

        if( size != itr->second )
        {
            AE_INFO_IF( report, "MemorySlots::release() - %zu bytes requested, but %zu bytes were reserved.", size, itr->second );
            return false;
        }

        // remove the reserved slot
        _reservedMemory.erase( itr );
        if( _offsetSizes.empty() )
        {
            _insert( offset, size );
            return true;
        }

        auto slotStart = offset;
        auto slotEnd   = offset + size;
        
        // merge with next slot if adjacent
        auto nextItr = _offsetSizes.upper_bound( offset );
        if( nextItr != _offsetSizes.end() && nextItr->first == slotEnd )
        {
            slotEnd = nextItr->first + nextItr->second;
            _remove( nextItr->first, nextItr->second );
        }
        
        // merge with previous slot if adjacent
        auto prevItr = nextItr != _offsetSizes.end() ? std::prev( nextItr ) : _offsetSizes.end();
        auto prevSlotEnd = prevItr->first + prevItr->second;
        if( prevSlotEnd == slotStart )
        {
            slotStart = prevItr->first;
            _remove( prevItr->first, prevItr->second );
        }

        // insert the merged slot back into the available memory
        _insert( slotStart, slotEnd - slotStart );
        return true;
    }

    void report() const
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

    bool check() const
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
};

} // namespace aer::mem
