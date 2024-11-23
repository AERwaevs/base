#pragma once

/* <editor-fold desc="MIT License">

Copyright( c) 2018 Robert Osfield

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

</editor-fold> */
#include <concepts>

#include "base_ptr.h"
#include "ref_ptr.h"

namespace aer
{

template< typename T >
struct spy_ptr : public mem::base_ptr<T>
{
    friend spy_ptr;
    friend ref_ptr<T>;
    
    using Base = mem::base_ptr<T>;
    using Base::ptr;

    spy_ptr() noexcept : Base() {}
    spy_ptr( const spy_ptr& ) noexcept = default;
    explicit spy_ptr( T* rhs ) noexcept : Base( rhs ) {}
    ~spy_ptr()                    = default;

    auto operator *     ( this auto& ) = delete;
    auto operator ->    ( this auto& ) = delete;

    ref_ptr<T> load() const { return ref_ptr<T>( ptr ); }
};

} // namespace aer