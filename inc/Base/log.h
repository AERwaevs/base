#pragma once

#include "../../lib/loguru/loguru.hpp"

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