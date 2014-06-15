// Copyright (C) 2008-2009 Harm Boschloo

#ifndef COCA_TYPE_H_INCLUDED
#define COCA_TYPE_H_INCLUDED

#include "config.h"

namespace coca
{
    ///@todo: define types for 64 bit or other platforms

#if defined COCA_WIN32
    typedef bool bool8;

    typedef signed char int8;
    typedef short int16;
    typedef int int32;
    typedef long long int64;

    typedef unsigned char uint8;
    typedef unsigned short uint16;
    typedef unsigned int uint32;
    typedef unsigned long long uint64;

    typedef float float32;
    typedef double float64;
#else
    #error TODO!
#endif

} // namespace coca

#endif // COCA_TYPE_H_INCLUDED
