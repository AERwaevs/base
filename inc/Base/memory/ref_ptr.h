#pragma once

/* <editor-fold desc="MIT License">

Copyright(c) 2018 Robert Osfield

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

</editor-fold> */
#include <concepts>

#include "base_ptr.h"

namespace aer
{

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

} // namespace aer