#pragma once

#include <Vendor/loguru/loguru.hpp>

namespace AEON::Log
{
    using namespace loguru;

    inline void Init(   int&    argc,
                        char**  argv )
    {
        g_preamble_header       = false;
        g_stderr_verbosity      = Verbosity_INFO;
        g_internal_verbosity    = Verbosity_MAX;
        g_flush_interval_ms     = 100;
        init( argc, argv );
    }

    inline void New(    const std::string&  log_name    = "AEON", 
                        FileMode            mode        = Truncate, 
                        Verbosity           verbosity   = Verbosity_INFO )
    {
        std::string file_name = log_name + ".log";
        add_file( file_name.c_str(), mode, verbosity ); 
    }
}

#define AE_INFO(                    ... )   LOG_F(    INFO,                 __VA_ARGS__ )
#define AE_WARN(                    ... )   LOG_F(    WARNING,              __VA_ARGS__ )
#define AE_ERROR(                   ... )   LOG_F(    ERROR,                __VA_ARGS__ )
#define AE_FATAL(                   ... )   ABORT_F(                        __VA_ARGS__ )
#define AE_INFO_IF(      condition, ... )   LOG_IF_F( INFO,     condition,  __VA_ARGS__ )
#define AE_WARN_IF(      condition, ... )   LOG_IF_F( WARNING,  condition,  __VA_ARGS__ )
#define AE_ERROR_IF(     condition, ... )   LOG_IF_F( ERROR,    condition,  __VA_ARGS__ )
#define AE_FATAL_IF(     condition, ... )   LOG_IF_F( FATAL,    condition,  __VA_ARGS__ )

#define AE_LOG(          verbosity, ... )   LOG_F(              verbosity,  __VA_ARGS__ )
#define AE_LOG_SCOPE(    verbosity, ... )   LOG_SCOPE_F(        verbosity,  __VA_ARGS__ )
#define AE_LOG_FUNCTION( verbosity      )   LOG_SCOPE_FUNCTION( verbosity )