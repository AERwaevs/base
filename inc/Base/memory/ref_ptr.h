#pragma once

/* <editor-fold desc="MIT License">

Copyright(c) 2018 Robert Osfield

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

</editor-fold> */
#include <concepts>

#include "base_ptr.h"

#define REF_PTR_VERSION 1

namespace aer
{

#if REF_PTR_VERSION == 1

template< typename T >
struct ref_ptr : public mem::base_ptr<T>
{
    friend ref_ptr;
    using Base = mem::base_ptr<T>;
    using Base::ptr;
    using Base::Base;

             ref_ptr()                     noexcept : Base() {}    
             ref_ptr( const ref_ptr& rhs ) noexcept : Base( rhs ) { if( ptr ) ptr->_ref(); }
    explicit ref_ptr( T* rhs )             noexcept : Base( rhs ) { if( ptr ) ptr->_ref(); }
            ~ref_ptr()                     noexcept               { if( ptr ) ptr->_unref(); }

    template< typename R > requires( std::derived_from<T, R> )
    operator ref_ptr<R>() const noexcept { return ref_ptr( ptr ); }

    T&     operator *     ( this auto& self  ) noexcept { return *self.ptr; }
    T*     operator ->    ( this auto& self  ) noexcept { return self.ptr; }
    
    ref_ptr& operator = ( const ref_ptr& rhs )
    {
        if( ptr != rhs.ptr )
        {
            T*  tmp_ptr = ptr;
                ptr     = rhs.ptr;
            if( ptr )     ptr->_ref();
            if( tmp_ptr ) tmp_ptr->_unref();
        }
        return *this;
    }
    
    ref_ptr& operator = ( T* rhs )
    {
        if( ptr != rhs )
        {
            T*  tmp_ptr     = ptr;
                ptr   = rhs;
            if( ptr ) ptr->_ref();
            if( tmp_ptr )   tmp_ptr->_unref();
        }
        return *this;
    }

    template< typename R > requires( std::derived_from<T, R> )
    ref_ptr& operator = ( const ref_ptr<R>& rhs )
    {
        if( ptr != rhs.ptr )
        {
            T*  tmp_ptr = ptr;
                ptr     = rhs.ptr;
            if( ptr )     ptr->_ref();
            if( tmp_ptr ) tmp_ptr->_unref();
        }
        return *this;
    }

    template< typename R > requires( std::derived_from<T, R> )
    ref_ptr& operator = ( ref_ptr<R>&& rhs )
    {
        if( ptr != rhs.ptr )
        {
            if( ptr )     ptr->_unref();
                ptr     = rhs.ptr;
                rhs.ptr = nullptr;
        }
        return *this;
    }
};

#elif REF_PTR_VERSION == 2

#include "managed_object.h"

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

#endif // REF_PTR_VERSION

} // namespace aer