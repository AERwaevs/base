#pragma once

namespace aer
{
    
template< typename T >
struct ptr_t
{
    using type = T;

    ptr_t()  : ptr( nullptr )    {}
    ~ptr_t()                     {}

    ptr_t( const ptr_t& rhs )  : ptr( nullptr )    {}
    
    auto operator <=> ( const ref_ptr& ) const = default;

    template< class R >
    auto operator <=> ( const R* rhs )   const { return ( ptr <=> rhs ); };
    
protected:
    T* ptr = nullptr;

};

} // namespace aer
