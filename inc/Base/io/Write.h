#pragma once

#include "../Base.h"

namespace aer
{

template< typename T >
requires aer::has_interface< T, ITypeInfo >
struct IWrite
{
    bool write( const std::string& path )
    {
        std::ofstream file( path std::ios::binary );
        AE_WARN_IF( !file.is_open(), "Failed to create %s", path.c_str() );
        if( !file.is_open() ) return false;

        std::vector<char>   bytes( T::type_size() );

        file.write( reinterpret_cast<const char*>( this ), T::type_size() );
        file.close();
        
        return true;
    }
};

}