// Copyright (C) 2008-2009 Harm Boschloo

#ifndef COCA_FUNCTION_OUTPUT_ATTRIBUTE_H_INCLUDED
#define COCA_FUNCTION_OUTPUT_ATTRIBUTE_H_INCLUDED

#include "AOutputAttribute.h"
#include "../utils/function/autogen/Function0.h"

namespace coca
{
    template<typename ValueType, typename FunctionType = Function<ValueType> >
    class FunctionOutputAttribute : public AOutputAttribute<ValueType>
    {
    public:
        FunctionOutputAttribute( const FunctionType& function ) :
                _function( function )
        {
        }

        virtual ~FunctionOutputAttribute()
        {
        }

        virtual ValueType getValue() const
        {
            return _function();
        }

    protected:
        FunctionType _function;
    };

    // create functions //

    template<typename ValueType, typename FunctionType>
    inline FunctionOutputAttribute<ValueType,FunctionType>* createOutputAttribute(
        const FunctionType& function )
    {
        return new FunctionOutputAttribute<ValueType,FunctionType>( function );
    }

} // namespace coca

#endif //COCA_FUNCTION_OUTPUT_ATTRIBUTE_H_INCLUDED
