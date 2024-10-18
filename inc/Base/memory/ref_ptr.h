#pragma once

#include <concepts>

#include "managed_object.h"

namespace aer {

template< typename T >
struct ref_ptr
{
    using element_t = T;
    using managed_t = mem::managed_object<T>;
    using pointer_t = std::add_pointer_t<managed_t>;

                ref_ptr()                     noexcept : ptr( nullptr ) {}
                ref_ptr( std::nullptr_t )     noexcept : ptr( nullptr ) {}
                ref_ptr( ref_ptr&& rhs )      noexcept : ptr( rhs.release() ) {}
                ref_ptr( const ref_ptr& rhs ) noexcept : ptr( rhs.ptr ) { if( ptr ) ptr->ref_hard(); }
    explicit    ref_ptr( pointer_t rhs )      noexcept : ptr( rhs )     { if( ptr ) ptr->ref_hard(); }
                ~ref_ptr()                    noexcept { clear(); }

    template< std::derived_from<T> R, typename derived_t = mem::managed_object<R> >
                ref_ptr( ref_ptr<R>&& rhs )      noexcept : ptr( std::move( *rhs.release() ) ) {}
    template< std::derived_from<T> R, typename derived_t = mem::managed_object<R> >
                ref_ptr( const ref_ptr<R>& rhs ) noexcept : ptr( new derived_t(rhs) ) { if( ptr ) ptr->ref_hard(); }
    template< std::derived_from<T> R, typename derived_t = mem::managed_object<R> >
    explicit    ref_ptr( R* rhs )                noexcept : ptr( new derived_t(rhs) ) { if( ptr ) ptr->ref_hard(); }

    inline bool valid()                 const noexcept { return ( ptr != nullptr ); }
    inline void swap( ref_ptr& other )  const noexcept { std::swap( ptr, other.ptr ); }
    inline void clear()                       noexcept { if( ptr ) ptr->unref_hard(); ptr = nullptr; }
    inline auto release()                     noexcept { auto p = ptr; clear(); return p; }

    operator    bool()                  const noexcept { return valid(); }
    template< typename Self >
    inline auto get( this Self&& self ) noexcept { return self.ptr->get(); }
    template< typename Self > std::add_pointer_t<T>
    operator ->( this Self&& self )     noexcept { return self.get(); }
    template< typename Self > std::add_lvalue_reference_t<T>
    operator * ( this Self&& self )     noexcept { return *(self.get()); }

    ref_ptr& operator = ( T* rhs )
    {
        if( ptr != rhs )
        {
            T*  tmp_ptr     = ptr;
                ptr   = rhs;
            if( ptr ) ptr->ref_hard();
            if( tmp_ptr )   tmp_ptr->unref_hard();
        }
        return *this;
    }

    ref_ptr& operator = ( const ref_ptr& rhs )
    {
        if( ptr != rhs.ptr )
        {
            T*  tmp_ptr = ptr;
                ptr     = rhs.ptr;
            if( ptr )     ptr->ref_hard();
            if( tmp_ptr ) tmp_ptr->unref_hard();
        }
        return *this;
    }

    template< class R >
    ref_ptr& operator = ( const ref_ptr<R>& rhs )
    {
        if( ptr != rhs.ptr )
        {
            T*  tmp_ptr = ptr;
                ptr     = rhs.ptr;
            if( ptr )     ptr->ref_hard();
            if( tmp_ptr ) tmp_ptr->unref_hard();
        }
        return *this;
    }

    template< class R >
    ref_ptr& operator = ( ref_ptr<R>&& rhs )
    {
        if( ptr != rhs.ptr )
        {
            if( ptr )     ptr->unref_hard();
                ptr     = rhs.ptr;
                rhs.ptr = nullptr;
        }
        return *this;
    }
    
protected:
    pointer_t ptr = nullptr;
};

} // namespace aer