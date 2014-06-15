// Copyright (C) 2008-2009 Harm Boschloo

#ifndef COCA_UTILS_H_INCLUDED
#define COCA_UTILS_H_INCLUDED

#include <coca/utils/IUtils.h>

namespace coca
{
    class Utils : public IUtils
    {
    public:
        Utils();
        ~Utils();

        void debugBreak();
    };

} // namespace coca

#endif // COCA_UTILS_H_INCLUDED
