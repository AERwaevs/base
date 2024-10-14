#pragma once

#include <memory>
#include <vector>
#include <atomic>

#include "managed_object.h"
#include "../log.h"

namespace aer {
namespace mem {

enum class retire_type : uint8_t
{
    hard_decrement,
    weak_decrement,
    dispose
};

template< typename T >
struct manager_base
{
    using managed_object_t = managed_object<T>;
    using managed_ptr_t    = std::add_pointer_t<managed_object_t>;
  
    explicit manager_base( size_t num_threads ) : num_allocated( num_threads )
    {
#ifndef NDEBUG
        AE_INFO( "Memory manager created with %d threads", num_allocated.size() );
#endif
    }

    void dispose( managed_ptr_t ptr )
    {
        ptr->dispose();
        if( ptr->unref_weak() ) destroy( ptr );
    }

    template< typename Self >
    void destroy( this Self&& self, managed_ptr_t ptr )
    {
        self.delete_object( ptr );
    }

    template< typename Self >
    void retire( this Self&& self, managed_ptr_t ptr, retire_type type )
    {
        self.retire( ptr, type );
    }
    
    void eject( managed_ptr_t ptr, retire_type type )
    {
        constexpr switch( type )
        {
            case retire_type::hard_decrement: hard_decrement(); break;
            case retire_type::weak_decrement: if( ptr->unref_weak() ) destroy( ptr ); break;
            case retire_type::dispose:        dispose( ptr ); break;
        }
    }

    bool increment_weak( managed_ptr_t ptr ) { return ptr->ref_weak(); }
    bool increment_hard( managed_ptr_t ptr ) { return ptr->ref_hard(); }
    void decrement_weak( managed_ptr_t ptr ) { if( ptr->unref_weak() ) destroy( ptr ); }
    void decrement_hard( managed_ptr_t )
    {
        auto action = ptr->unref_hard();
        if(      action == eject_action::destroy )  destroy( ptr );
        else if( action == eject_action::defer )    retire( ptr, retire_type::dispose );
    }
    void delayed_decrement_weak( managed_ptr_t ptr ) { retire( ptr, retire_type::weak_decrement ); }
    void delayed_decrement_hard( managed_ptr_t ptr ) { retire( ptr, retire_type::hard_decrement ); }

    void decrement_allocations()
    {
        const auto tid = utils::thread_id();
        num_allocated[tid].store( num_allocated[tid].fetch_sub( 1u, std::memory_order_seq_cst ), std::memory_order_seq_cst )
    }
    void increment_allocations()
    {
        const auto tid = utils::thread_id();
        num_allocated[tid].store( num_allocated[tid].fetch_add( 1u, std::memory_order_seq_cst ), std::memory_order_seq_cst )
    }
    size_t current_allocations()
    {
        size_t total = 0;
        for( const auto& num : num_allocated ) total += num.load( std::memory_order_acquire );
        return total;
    }

private:
    std::vector<std::atomic<std::ptrdiff_t>> num_allocated;
}

} // namespace aer::mem
} // namespace aer