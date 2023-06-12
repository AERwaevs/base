#pragma once

#include "../Base.h"

namespace aer
{

template< typename T >
struct IRead
{
    static ref_ptr<T> read( const std::string& path )
    {
        std::ifstream file( path, std::ios::ate | std::ios::binary );
        AE_FATAL_IF( !file.is_open(), "Failed to open %s", path.c_str() );

        std::size_t         file_size = static_cast<std::size_t>( file.tellg() );
        std::vector<char>   bytes( file_size );

        file.seekg(0);
        file.read( bytes.data(), file_size );
        file.close();

        return ref_ptr<T>( reinterpret_cast<T*>( bytes.data() ) );
    }
};

}