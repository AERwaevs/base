#pragma once

#include <stdint.h>

namespace aer
{
namespace mem
{

enum MemoryTracking : uint8_t
{
    MEMORY_TRACKING_NO_CHECKS       = 0,
    MEMORY_TRACKING_REPORT_ACTIONS  = 1,
    MEMORY_TRACKING_CHECK_ACTIONS   = 2,
    MEMORY_TRACKING_DEFAULT         = MEMORY_TRACKING_NO_CHECKS
};

} // namespace aer::mem
} // namespace aer