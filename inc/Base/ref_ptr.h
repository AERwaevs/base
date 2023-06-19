#pragma once

/* <editor-fold desc="MIT License">

Copyright(c) 2018 Robert Osfield

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

</editor-fold> */
#include <concepts>

#include "object.h"

namespace aer
{

template< typename T >
class ref_ptr
{
public:
    using type = T;

    ref_ptr()                        noexcept : ptr( nullptr ) {}
    ~ref_ptr()                       noexcept                  { if( ptr ) ptr->_unref(); }
    
    ref_ptr( const ref_ptr& rhs )    noexcept : ptr( rhs.ptr ) { if( ptr ) ptr->_ref(); }

    template< class R >
    ref_ptr( ref_ptr<R>&& rhs )      noexcept : ptr( rhs.ptr ) { rhs.ptr = nullptr; }

    template< class R >
    ref_ptr( const ref_ptr<R>& rhs ) noexcept : ptr( rhs.ptr ) { if( ptr ) ptr->_ref(); }

    explicit ref_ptr( T* rhs )       noexcept : ptr( rhs )     { if( ptr ) ptr->_ref(); }

    template< class R >
    explicit ref_ptr( R* rhs )       noexcept : ptr( rhs )     { if( ptr ) ptr->_ref(); }

    ref_ptr& operator = ( T* rhs )
    {
        if( ptr != rhs )
        {
            T*  tmp_ptr = ptr;
                ptr     = rhs;
            if( ptr )     ptr->_ref();
            if( tmp_ptr ) tmp_ptr->_unref();
        }
        return *this;
    }

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

    template< class R >
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

    template< class R >
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

    template< class R >
    bool operator <  ( const ref_ptr<R>& rhs ) const { return ( ptr <  rhs.ptr ); }

    template< class R >
    bool operator >  ( const ref_ptr<R>& rhs ) const { return ( ptr >  rhs.ptr ); }

    template< class R >
    bool operator == ( const ref_ptr<R>& rhs ) const { return ( ptr == rhs.ptr ); }

    template< class R >
    bool operator != ( const ref_ptr<R>& rhs ) const { return ( ptr != rhs.ptr ); }

    template< class R >
    bool operator <  ( const R* rhs )          const { return ( ptr <  rhs ); }

    template< class R >
    bool operator >  ( const R* rhs )          const { return ( ptr >  rhs ); }

    template< class R >
    bool operator == ( const R* rhs )          const { return ( ptr == rhs ); }

    template< class R >
    bool operator != ( const R* rhs )          const { return ( ptr != rhs ); }

    bool valid()                      const noexcept { return ( ptr != nullptr ); }

    explicit operator bool  ()        const noexcept { return valid(); }

    explicit operator T*    ()        const noexcept { return ptr; }

    T* operator ->()                  const noexcept { return ptr; }

    T* get()                          const noexcept { return ptr; }

    T& operator *()                   const noexcept { return *ptr; }

    void operator []( int )           const = delete;

    void swap( ref_ptr& rhs ) noexcept
    {
        T*  tmp_ptr = ptr;
            ptr     = rhs.ptr;
            rhs.ptr = tmp_ptr;
    }

    template< class R >
    ref_ptr<R> cast() const { return ref_ptr<R>( valid() ? ptr->template cast<R>() : nullptr ); }

protected:
    template< class R >
    friend class    ref_ptr;
    
protected:
    T* ptr = nullptr;
};

} // namespace aer