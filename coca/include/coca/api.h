// Copyright (C) 2008-2009 Harm Boschloo

#ifndef COCA_API_H_INCLUDED
#define COCA_API_H_INCLUDED

#include "utils/dll.h"

#if !defined(__cplusplus)
#error C++ compiler required.
#endif

#ifdef COCA_DLL_BUILD
#define COCA_API extern "C" COCA_DLL_EXPORT
#else
#define COCA_API extern "C" COCA_DLL_IMPORT
#endif

#endif // COCA_API_H_INCLUDED
