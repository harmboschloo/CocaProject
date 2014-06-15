// Copyright (C) 2008-2009 Harm Boschloo

#ifndef COCA_DLL_H_INCLUDED
#define COCA_DLL_H_INCLUDED

#include "config.h"

// dll export/import configuration
#if defined(COCA_WIN32)
    #define COCA_DLL_EXPORT __declspec(dllexport)
    #define COCA_DLL_IMPORT __declspec(dllimport)
#elif defined(COCA_LINUX)
    #define COCA_DLL_EXPORT __attribute__((visibility("default")))
    #define COCA_DLL_IMPORT
#else
    #error undefined, please specify or report
#endif

#endif // COCA_DLL_H_INCLUDED
