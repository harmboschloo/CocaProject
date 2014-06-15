// Copyright (C) 2008-2009 Harm Boschloo

#ifndef COCA_VOID_OUTPUT_ATTRIBUTE_H_INCLUDED
#define COCA_VOID_OUTPUT_ATTRIBUTE_H_INCLUDED

#include "AOutputAttribute.h"

namespace coca
{
    class VoidOutputAttribute : public AOutputAttribute<void>
    {
    public:
        VoidOutputAttribute()
        {
        }

        ~VoidOutputAttribute()
        {
        }
    };

    // create functions //

    inline VoidOutputAttribute* createOutputAttribute()
    {
        return new VoidOutputAttribute();
    }

} // namespace coca

#endif //COCA_FUNCTION_OUTPUT_ATTRIBUTE_H_INCLUDED
