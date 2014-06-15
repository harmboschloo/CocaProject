// Copyright (C) 2008-2009 Harm Boschloo

#ifndef COCA_STATIC_CAST_H_INCLUDED
#define COCA_STATIC_CAST_H_INCLUDED

#if defined(NDEBUG)

namespace coca
{
    template<typename T1, typename T2>
    inline T1 staticCast( T2 x2 ) { return static_cast<T1>( x2 ); }

} // namespace coca

#else // if !defined(NDEBUG)

#include "test.h"

namespace coca
{
    template<typename T1, typename T2>
    T1 staticCast( T2 x2 )
    {
        T1 x1 = dynamic_cast<T1>( x2 );
        if ( x2 ) { COCA_ASSERT( x1 ); }
        return x1;
    }

} // namespace coca

#endif // NDEBUG

#endif // COCA_STATIC_CAST_H_INCLUDED

