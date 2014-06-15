// Copyright (C) 2008-2009 Harm Boschloo

#include "Utils.h"

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

namespace coca
{
    Utils::Utils()
    {
    }

    Utils::~Utils()
    {
    }

    void Utils::debugBreak()
    {
        ::DebugBreak();
    }

} // namespace coca
