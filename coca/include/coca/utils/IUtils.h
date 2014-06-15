// Copyright (C) 2008-2009 Harm Boschloo

#ifndef COCA_I_MANAGER_H_INCLUDED
#define COCA_I_MANAGER_H_INCLUDED

#include "../interface.h"

namespace coca
{
    class IUtils
    {
    public:
        virtual void debugBreak() = 0;

    protected:
        virtual ~IUtils() {}
    };

} // namespace coca

#endif // COCA_I_MANAGER_H_INCLUDED
