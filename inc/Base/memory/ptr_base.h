#pragma once

#include <memory>
#include <concepts>

namespace aer
{
    
template< typename T >
struct ptr_t
{
    using type = T;

                ptr_t()                   noexcept : ptr( nullptr ) {}
                ptr_t( const ptr_t& rhs ) noexcept : ptr( rhs.ptr ) {}
    explicit    ptr_t( T* rhs )           noexcept : ptr( rhs )     {}
                ~ptr_t()                                            {}
    
    template< class R >
    auto        operator <=> ( const R* rhs )   const noexcept { return ( ptr <=> rhs ); };
    auto        operator <=> ( const ptr_t& )   const = default;
    explicit    operator bool()                 const noexcept { return ( ptr != nullptr ); }
                operator T*  ()                 const noexcept { return   ptr; }
    T*          get()                           const noexcept { return   ptr; }
    T*          operator ->  ()                 const noexcept { return   ptr; }
    T&          operator *   ()                 const noexcept { return * ptr; }
    void        operator []  ( int )            const = delete;
    
    bool        valid()                         const noexcept { return operator bool(); }
    void        swap( ptr_t& other )                           { std::swap( ptr, other.value ); }

protected:
    template< class > friend class ptr_t;
    T* ptr = nullptr;
};

} // namespace aer
