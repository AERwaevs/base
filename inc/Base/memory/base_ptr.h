#pragma once

#include <memory>
#include <type_traits>

namespace aer
{
namespace mem
{
    
template< typename U >
requires std::is_pointer_v<U>
struct base_ptr
{
    using ptr_t     = U;
    using value_t   = std::remove_pointer_t<U>;

                base_ptr()                          noexcept : ptr( nullptr )   {}
    explicit    base_ptr( U rhs )                   noexcept : ptr( rhs )       {}
                base_ptr( base_ptr&& rhs )          noexcept : ptr( rhs.ptr )   {}
                ~base_ptr()                                                     {}
    
    explicit    operator bool   ()                  const noexcept { return ( ptr != nullptr ); }
    explicit    operator U      ()                  const noexcept { return   ptr; }
    U           operator ->     ()                  const noexcept { return   ptr; }
    value_t&    operator *      ()                  const noexcept { return  *ptr; }
    base_ptr&   operator =      ( base_ptr&& rhs )  const noexcept { ptr = rhs.ptr; rhs.ptr = nullptr; }
    void        operator []     ( int )             const = delete;
    template< class R >
    auto        operator <=>    ( const R* rhs )    const noexcept { return ( ptr <=> rhs ); };
    auto        operator <=>    ( const base_ptr& ) const = default;

    U           get()                               const noexcept { return   ptr; }
    U&          get()                                     noexcept { return   ptr; }
    bool        valid()                             const noexcept { return ( ptr != nullptr ); }
    void        swap( U rhs )                       const noexcept { std::swap( ptr, rhs ); }
    void        swap( base_ptr& rhs )               const noexcept { std::swap( ptr, rhs.ptr ); }
protected:
    void        set( U rhs )                              noexcept { ptr = rhs; }
    void        clear()                                   noexcept { ptr = nullptr; }
private:
    U ptr = nullptr;
};

} // namespace aer::mem
} // namespace aer
