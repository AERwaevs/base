#pragma once

#include <memory>
#include <type_traits>

namespace aer
{
namespace mem
{
    
template< typename Ptr >
requires std::is_pointer_v<Ptr>
struct base_ptr
{
    using type = std::remove_pointer_t<Ptr>;

                base_ptr()                      noexcept : ptr( nullptr ) {}
                base_ptr( const base_ptr& rhs ) noexcept : ptr( rhs.ptr ) {}
    explicit    base_ptr( Ptr rhs )             noexcept : ptr( rhs )     {}
                ~base_ptr()                                               {}
    
    template< class R >
    auto        operator <=>    ( const R* rhs )    const noexcept { return ( ptr <=> rhs ); };
    auto        operator <=>    ( const base_ptr& ) const = default;
    explicit    operator bool   ()                  const noexcept { return ( ptr != nullptr ); }
                operator type*  ()                  const noexcept { return   ptr; }
    type*       operator ->     ()                  const noexcept { return   ptr; }
    type&       operator *      ()                  const noexcept { return  *ptr; }
    Ptr         get()                               const noexcept { return   ptr; }
    Ptr&        get()                                     noexcept { return   ptr; }
    void        operator []     ( int )             const = delete;

    //[[nodiscard]]
    //bool        is_protected()                      const          { return true; }
    bool        valid()                             const noexcept { return ( ptr != nullptr ); }
    //void        clear()                                   noexcept { ptr = nullptr; }
    friend void swap( base_ptr& other )                            { std::swap( ptr, other.ptr ); }

protected:
    Ptr ptr = nullptr;
};

} // namespace aer::mem
} // namespace aer
