#pragma once

#include <algorithm>

#include "../object.h"
#include "ref_ptr.h"

#include "../interfaces/create.h"

namespace aer
{
    
struct scratch_memory : public inherit<scratch_memory, Object>
{
    uint8_t* buffer = nullptr;
    uint8_t* ptr    = nullptr;
    size_t   size   = 0;

    scratch_memory* next;

    explicit scratch_memory( size_t in_size ) : buffer( new uint8_t[in_size] ), ptr( buffer ), size( in_size ) {};
            ~scratch_memory() { delete[] buffer; }
             scratch_memory( const scratch_memory& ) = delete;
             scratch_memory( scratch_memory&& rhs ) : buffer( new uint8_t[rhs.size] ), ptr( buffer ), size( rhs.size ) {};
             
    scratch_memory& operator = ( const scratch_memory& ) = delete;

    uint8_t* align( uint8_t* ptr ) const
    {
        auto alignment = sizeof( ptr );
        auto ptr_pos = ( ( reinterpret_cast<size_t>( ptr ) + alignment - 1 ) / alignment ) * alignment;
        return reinterpret_cast<uint8_t*>( ptr_pos );
    }

    template< typename T >
    T* allocate( size_t num = 1 )
    {
        if( num == 0 ) return nullptr;

        size_t allocate_size = sizeof(T) * num;
        if( ( ptr + allocate_size ) <= ( buffer + size ) )
        {
            T* allocated_ptr = reinterpret_cast<T*>( ptr );
            ptr = align( ptr + allocate_size );
            return allocated_ptr;
        }

        if( !next ) next = new scratch_memory( std::max( size, allocate_size ) );
        return next->allocate<T>( num );
    }

    void release()
    {
        ptr = buffer;
        if( next ) next->release();
    }
};

} // namespace aer
