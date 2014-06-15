// Copyright (C) 2008-2009 Harm Boschloo

#ifndef COCA_FUNCTION_INPUT_ATTRIBUTE_H_INCLUDED
#define COCA_FUNCTION_INPUT_ATTRIBUTE_H_INCLUDED

#include "AInputAttribute.h"
#include "../utils/string.h"
#include "../utils/function/autogen/Function0.h"
#include "../utils/function/autogen/Function1.h"

namespace coca
{
    template<typename ValueType, typename FunctionType = Function<void,ValueType> >
    class FunctionInputAttribute : public AInputAttribute<ValueType>
    {
    public:
        FunctionInputAttribute( const FunctionType& function ) :
                AInputAttribute<ValueType>( false ),
                _function( function )
        {
        }

        virtual ~FunctionInputAttribute()
        {
        }

    protected:
        virtual void onSourceAttach( IAttribute& output )
        {
            onSourceUpdate( output );
        }

        virtual bool onSourceUpdate( IAttribute& output )
        {
            _function( this->getSourceAttribute( output ).getValue() );
            return true;
        }

        virtual bool onStringUpdate( const std::string& valueString )
        {
            ValueType value;
            if ( !fromString( valueString, value ) ) { return false; }
            _function( value );
            return true;
        }

        FunctionType _function;
    };

    // pointer partial specialization

    template<typename ValueType, typename FunctionType>
    class FunctionInputAttribute<ValueType*,FunctionType> : public AInputAttribute<ValueType*>
    {
    public:
        FunctionInputAttribute( const FunctionType& function ) :
                AInputAttribute<ValueType*>( false ),
                _function( function )
        {
        }

        virtual ~FunctionInputAttribute()
        {
        }

    protected:
        virtual void onSourceAttach( IAttribute& output )
        {
            onSourceUpdate( output );
        }

        virtual void onSourceDetach( IAttribute& COCA_HIDE( output ) )
        {
            _function( 0 );
        }

        virtual bool onSourceUpdate( IAttribute& output )
        {
            _function( this->getSourceAttribute( output ).getValue() );
            return true;
        }

        FunctionType _function;
    };

    // void partial specialization

    template<typename FunctionType>
    class FunctionInputAttribute<void,FunctionType> : public AInputAttribute<void>
    {
    public:
        FunctionInputAttribute( const FunctionType& function, bool multipleSources = false ) :
                AInputAttribute<void>( multipleSources ),
                _function( function )
        {
        }

        virtual ~FunctionInputAttribute()
        {
        }

    protected:
        virtual bool onSourceUpdate( IAttribute& COCA_HIDE( output ) )
        {
            _function();
            return true;
        }

        virtual bool onStringUpdate( const std::string& COCA_HIDE( valueString ) )
        {
            _function();
            return true;
        }

        FunctionType _function;
    };

    // create functions

    template<typename ValueType, typename FunctionType>
    inline FunctionInputAttribute<ValueType,FunctionType>* createInputAttribute(
        const FunctionType& function )
    {
        return new FunctionInputAttribute<ValueType,FunctionType>( function );
    }

    template<typename FunctionType>
    inline FunctionInputAttribute<FunctionType>* createInputAttribute(
        const FunctionType& function, bool multipleSources = false )
    {
        return new FunctionInputAttribute<FunctionType>( function, multipleSources );
    }

} // namespace coca

#endif //COCA_FUNCTION_INPUT_ATTRIBUTE_H_INCLUDED
