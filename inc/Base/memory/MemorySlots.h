#pragma once

#include <numeric>
#include <optional>
#include <map>

#include "MemoryTracking.h"

namespace aer
{
namespace mem
{

// this is purely for readability
using offset_t = size_t;

struct MemorySlot : private std::pair<offset_t, size_t>
{
    using std::pair<offset_t, size_t>::pair;

    auto offset() const { return first; }
    auto size()   const { return second; }
};

// Manages sub-allocations within a block of memory.
struct MemorySlots
{
    // default alignment is 4 bytes or 64 bits
    constexpr static size_t DEFAULT_ALIGNMENT = 4;

    // whether to report or check memory actions, can flood the log with messages however
    mutable MemoryTracking memoryTracking;
    
    MemorySlots( size_t availableMemorySize, MemoryTracking tracking = MEMORY_TRACKING_DEFAULT );

    // Returns the offset of the reserved memory, or std::nullopt if no memory is available.
    std::optional<offset_t> reserve( size_t size, size_t alignment = DEFAULT_ALIGNMENT );
    // Releases the memory at the given offset, returning true if the memory was reserved.
    bool release( offset_t offset, size_t size );

    bool full()  const { return _availableMemory.empty(); }
    bool empty() const { return totalAvailableSize() == totalMemorySize(); }
    bool check() const;
    void report() const;

    size_t totalMemorySize() const { return _totalMemorySize; }
    size_t totalAvailableSize() const
    {
        return std::accumulate( _availableMemory.begin(), _availableMemory.end(), 0, []( auto sum, auto& slot ) { return sum + slot.first; } );
    }
    size_t totalReservedSize() const
    {
        return std::accumulate( _reservedMemory.begin(), _reservedMemory.end(), 0, []( auto sum, auto& slot ) { return sum + slot.second; } );
    }
protected:
    void insertAvailableSlot( offset_t, size_t );
    void removeAvailableSlot( offset_t, size_t );
private:
    std::multimap<size_t, offset_t> _availableMemory;
    std::map<offset_t, size_t>      _reservedMemory;
    std::map<offset_t, size_t>      _offsetSizes;
    size_t                          _totalMemorySize;
};

} // namespace aer::mem
} // namespace aer
