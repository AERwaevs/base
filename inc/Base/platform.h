#ifdef __clang__
#   define AER_COMPILER_CLANG
#elif __GNUC__
#   define AER_COMPILER_GNU
#elif _MSC_VER
#   define AER_COMPILER_MSC
#elif __MINGW64__
#   define AER_COMPILER_MINGW
#endif
#ifdef _WIN32
#   ifdef _WIN64
#       ifndef NOMINMAX 
#           define NOMINMAX    /* Guard against redefining min and max macros on Windows */
#       endif
#       ifdef AER_SHARED
#           ifdef AER_EXPORTS
#               define AER_DLL __declspec(dllexport)  /* Export this symbol from .dll */
#           else
#               define AER_DLL __declspec(dllimport)  /* Import this symbol from .dll */
#           endif
#           define     AER_API __cdecl                /* Specifies API entry into .dll  */
#       else
#       define AER_DLL
#       define AER_API
#       endif
#       define     AER_PLATFORM_WINDOWS
#   else
#       error "AER does not support 32 bit!"
#   endif
#else
#   include <cxxabi.h>    // for __cxa_demangle
#   define CXX_DEMANGLE 1
#   define         AER_DLL
#   define         AER_API
#   ifdef __APPLE__
#       include <TargetConditionals.h>
#       if TARGET_OS_IPHONE == 1
#           define AER_PLATFORM_IOS
#           error "iOS is not supported!"
#       elif TARGET_OS_MAC == 1
#           define AER_PLATFORM_MACOS
#           error "MacOS is not supported!"
#       else
#           error "Unknown Apple platform!"
#       endif
#   elif __ANDROID__
#       define     AER_PLATFORM_ANDROID
#       error "Android is not supported!"
#   elif __linux__
#       define     AER_PLATFORM_LINUX
#   else
#       error "Unknown platform!"
#   endif
#endif