#include <memory/manager_base.h>

namespace aer {
namespace mem {

// T =              The underlying type of the object being protected
// snapshot_slots = The number of additional announcement slots available for
//                  snapshot pointers. More allows more snapshots to be alive
//                  at a time, but makes reclamation slower
// eject_delay =    The maximum number of deferred ejects that will be held by
//                  any one worker thread is at most eject_delay * #threads.
//
template< typename T, size_t snapshot_slots = 7, size_t eject_delay = 2 >
struct acquire_retire : public manager_base< T >
{
    using base = manager_base<T>;
    using base::increment_allocations;
    using base::decrement_allocations;
    using base::increment_hard;
    using base::decrement_weak;
    using base::eject;

    using managed_object_t = typename base::managed_object_t;
    using managed_ptr_t    = typename base::managed_ptr_t;

    explicit acquire_retire( size_t num_threads ) : base( num_threads ) {}

    void retire( managed_ptr_t ptr, retire_type type ) override
    {
        if( ptr->unref_hard() == eject_action::destroy ) base_t::destroy( ptr );
        else if( type == retire_type::weak_decrement && ptr->unref_weak() ) base_t::destroy( ptr );
    }

    void eject( managed_ptr_t ptr, retire_type type ) override
    {
        base_t::eject( ptr, type );
    }

    std::vector<
};

}} // namespace aer::mem