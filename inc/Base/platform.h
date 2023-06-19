#ifdef __clang__
#   define AEON_COMPILER_CLANG
#elif __GNUC__
#   define AEON_COMPILER_GNU
#elif _MSC_VER
#   define AEON_COMPILER_MSC
#elif __MINGW64__
#   define AEON_COMPILER_MINGW
#endif
#ifdef _WIN32
#   ifdef _WIN64
#       ifndef NOMINMAX 
#           define NOMINMAX    /* Guard against redefining min and max macros on Windows */
#       endif
#       ifdef AEON_SHARED
#           ifdef AEON_EXPORTS
#               define AEON_DLL __declspec(dllexport)  /* Export this symbol from .dll */
#           else
#               define AEON_DLL __declspec(dllimport)  /* Import this symbol from .dll */
#           endif
#           define     AEON_API __cdecl                /* Specifies API entry into .dll  */
#       else
#       define AEON_DLL
#       define AEON_API
#       endif
#       define     AEON_PLATFORM_WINDOWS
#   else
#       error "AEON does not support 32 bit!"
#   endif
#else
#   include <cxxabi.h>    // for __cxa_demangle
#   define CXX_DEMANGLE 1
#   define         AEON_DLL
#   define         AEON_API
#   ifdef __APPLE__
#       include <TargetConditionals.h>
#       if TARGET_OS_IPHONE == 1
#           define AEON_PLATFORM_IOS
#           error "iOS is not supported!"
#       elif TARGET_OS_MAC == 1
#           define AEON_PLATFORM_MACOS
#           error "MacOS is not supported!"
#       else
#           error "Unknown Apple platform!"
#       endif
#   elif __ANDROID__
#       define     AEON_PLATFORM_ANDROID
#       error "Android is not supported!"
#   elif __linux__
#       define     AEON_PLATFORM_LINUX
#       error "Linux is not supported!"
#   else
#       error "Unknown platform!"
#   endif
#endif