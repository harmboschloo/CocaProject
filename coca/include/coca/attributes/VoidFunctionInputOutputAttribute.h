// Copyright (C) 2008-2009 Harm Boschloo

#ifndef COCA_VOID_FUNCTION_INPUT_OUTPUT_ATTRIBUTE_H_INCLUDED
#define COCA_VOID_FUNCTION_INPUT_OUTPUT_ATTRIBUTE_H_INCLUDED


#include "AInputOutputAttribute.h"
#include "../utils/function/autogen/Function0.h"

namespace coca
{
    template<typename FunctionType = Function<> >
    class VoidFunctionInputOutputAttribute : public AInputOutputAttribute<void>
    {
    public:
        VoidFunctionInputOutputAttribute( const FunctionType& function, bool multipleSources = false ) :
                AInputOutputAttribute<void>( multipleSources ),
                _function( function )
        {
        }

        virtual ~VoidFunctionInputOutputAttribute()
        {
        }

    protected:
        virtual bool onSourceUpdate( IAttribute& output )
        {
            _function();
            return true;
        }

        virtual bool onStringUpdate( const std::string& valueString )
        {
            _function();
            return true;
        }

        FunctionType _function;
    };

    // create function //

    template<typename FunctionType>
    inline VoidFunctionInputOutputAttribute<FunctionType>* createInputOutputAttribute(
        const FunctionType& function, bool multipleSources = false )
    {
        return new VoidFunctionInputOutputAttribute<FunctionType>( function, multipleSources );
    }

} // namespace coca

#endif //COCA_VOID_INPUT_OUTPUT_ATTRIBUTE_H_INCLUDED
