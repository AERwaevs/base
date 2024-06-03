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
struct ref_ptr : public mem::base_ptr< std::add_pointer_t<T> >
{
    template< class R > friend class ref_ptr;

    using ptr_t     = mem::base_ptr<std::add_pointer_t<T>>;
    using value_t   = T;

    using ptr_t::valid;
    using ptr_t::get;

                ref_ptr()                        noexcept : ptr_t( nullptr )   {}
                ref_ptr( const ref_ptr& rhs )    noexcept : ptr_t( rhs.get() ) { if( valid() ) get()->_ref(); }
    explicit    ref_ptr( T* rhs )                noexcept : ptr_t( rhs )       { if( valid() ) get()->_ref(); }
                ~ref_ptr()                       noexcept                      { if( valid() ) get()->_unref(); }

    template< class R >
                ref_ptr( ref_ptr<R>&& rhs )      noexcept : ptr_t( rhs.get() ) {}
    template< class R >
                ref_ptr( const ref_ptr<R>& rhs ) noexcept : ptr_t( rhs.get() ) { if( valid() ) get()->_ref(); }
    template< class R >
    explicit    ref_ptr( R* rhs )                noexcept : ptr_t( rhs )       { if( valid() ) get()->_ref(); }

    template< typename... Args > requires std::constructible_from< T, Args... > static constexpr
    ref_ptr<T> create( Args&&... args ) 
    {
        return ref_ptr( new T( std::forward<Args>( args )... ) );
    }

    ref_ptr& operator = ( T* rhs )
    {
        if( get() != rhs ) ptr_t::swap( rhs );
        return *this;
    }

    ref_ptr& operator = ( const ref_ptr& rhs )
    {
        if( get() != rhs.ptr ) ptr_t::swap( rhs );
        return *this;
    }

    template< class R >
    ref_ptr& operator = ( const ref_ptr<R>& rhs )
    {
        if( get() != rhs.ptr ) ptr_t::swap( rhs );
        return *this;
    }

    template< class R >
    ref_ptr& operator = ( ref_ptr<R>&& rhs )
    {
        if( get() != rhs.get() )
        {
            if( valid() ) get()->_unref();
            ptr_t::set( rhs.get() );
        }
        return *this;
    }
};

} // namespace aer