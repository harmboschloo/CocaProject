// Copyright (C) 2008-2009 Harm Boschloo

#ifndef COCA_DEBUG_BREAK_H_INCLUDED
#define COCA_DEBUG_BREAK_H_INCLUDED

#include "IUtils.h"

namespace coca
{
    inline void debugBreak() { getUtils().debugBreak(); }

} // namespace coca

#endif // COCA_DEBUG_BREAK_H_INCLUDED
