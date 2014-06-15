// Copyright (C) 2008-2009 Harm Boschloo

#ifndef COCA_A_OUTPUT_ATTRIBUTE_H_INCLUDED
#define COCA_A_OUTPUT_ATTRIBUTE_H_INCLUDED

#include "ATypeAttribute.h"

namespace coca
{
    template<typename ValueType>
    class AOutputAttribute : public ATypeAttribute<ValueType>
    {
    public:
        AOutputAttribute()
        {
            this->initSinks();
        }

        virtual ~AOutputAttribute()
        {
        }

        virtual ValueType getValue() const = 0;

    protected:
        virtual std::string onStringRequest() const
        {
            return toString( getValue() );
        }

    };

    // void specialization //

    template<>
    class AOutputAttribute<void> : public ATypeAttribute<void>
    {
    public:
        AOutputAttribute()
        {
            this->initSinks();
        }

        virtual ~AOutputAttribute()
        {
        }
    };


} // namespace coca

#endif //COCA_A_OUTPUT_ATTRIBUTE_H_INCLUDED
