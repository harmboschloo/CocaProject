// Copyright (C) 2008-2009 Harm Boschloo

#ifndef COCA_A_TYPE_ATTRIBUTE_H_INCLUDED
#define COCA_A_TYPE_ATTRIBUTE_H_INCLUDED

#include "AAttribute.h"

namespace coca
{
    template<typename ValueType>
    class ATypeAttribute : public AAttribute
    {
    public:
        ATypeAttribute()
        {
        }

        virtual ~ATypeAttribute()
        {
        }

        virtual const std::type_info& onTypeRequest() const
        {
            return typeid( ValueType );
        }
    };

    // pointer partial specialization

    template<typename ValueType>
    class ATypeAttribute<ValueType*> : public AAttribute
    {
    public:
        ATypeAttribute()
        {
        }

        virtual ~ATypeAttribute()
        {
        }

        virtual bool canBeSetAsString() const
        {
            return false;
        }

        virtual const std::type_info& onTypeRequest() const
        {
            return typeid( ValueType* );
        }
    };

} // namespace coca

#endif //COCA_A_TYPE_ATTRIBUTE_H_INCLUDED
