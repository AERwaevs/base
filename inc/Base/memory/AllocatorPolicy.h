#pragma once

namespace aer
{

namespace mem
{ 

enum AllocatorPolicy : uint8_t
{
    ALLOCATOR_POLICY_NO_DELETE = 0,
    ALLOCATOR_POLICY_STD_NEW_DELETE,
    ALLOCATOR_POLICY_STD_MALLOC_FREE,
    ALLOCATOR_POLICY_AER_ALLOC_DEALLOC,
    ALLOCATOR_POLICY_AER_ACQUIRE_RETIRE,
    ALLOCATOR_POLICY_DEFAULT = ALLOCATOR_POLICY_STD_NEW_DELETE
};

} // namespace aer::mem
} // namespace aer
