// Copyright (C) 2008-2009 Harm Boschloo

#ifndef COCA_CONFIG_H_INCLUDED
#define COCA_CONFIG_H_INCLUDED

#include "ECompilerType.h"
#include "EPlatformType.h"

/// @todo: full platform, copiler, ... config
/// (alse see boost\config header files and
/// http://www.boost.org/doc/libs/1_38_0/libs/config/doc/html/index.html)

// platform configuration
#if defined(linux) || defined(__linux) || defined(__linux__)
    #define COCA_LINUX
    #define COCA_PLATFORM coca::E_LINUX_PLATFORM
#elif defined(_WIN32) || defined(__WIN32__) || defined(WIN32)
    #define COCA_WIN32
    #define COCA_PLATFORM coca::E_WIN32_PLATFORM
#else
    #error undefined platform, please specify or report
#endif

// compiler configuration
/// @todo: add specific compiler AND compiler version! ( __MINGW32__, ... )
# if defined __GNUC__
    #define COCA_GNUC
    #define COCA_COMPILER coca::E_GNUC_COMPILER
#elif defined _MSC_VER
    #define COCA_MSC
    #define COCA_COMPILER coca::E_MSC_COMPILER
#else
    #error undefined compiler, please specify or report
#endif

#endif // COCA_CONFIG_H_INCLUDED
