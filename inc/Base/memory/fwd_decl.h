#pragma once

namespace aer {
namespace mem {

class Allocator;
class MemoryBlocks;
class Object;

template< typename T >
using default_memory_manager = aer::mem::acquire_retire<T>;

struct default_pointer_policy
{
    template< typename T >
    using pointer_type = std::add_pointer_t<T>;
};

template< typename T, typename manager = default_memory_manager<T>, typename policy = default_pointer_policy >
struct ref_ptr;

template< typename T, typename manager = default_memory_manager<T>, typename policy = default_pointer_policy >
struct arc_ptr;

template< typename T, typename manager = default_memory_manager<T>, typename policy = default_pointer_policy >
struct weak_ptr;



}} // namespace aer::mem