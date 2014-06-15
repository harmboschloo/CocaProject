// Copyright (C) 2008-2009 Harm Boschloo

#ifndef COCA_EVENT_INPUT_ATTRIBUTE_H_INCLUDED
#define COCA_EVENT_INPUT_ATTRIBUTE_H_INCLUDED

#include "AInputAttribute.h"
#include "AttributeEvent.h"
#include "../utils/function/autogen/Function1.h"

namespace coca
{
    template<typename ValueType, typename FunctionType = Function<bool,const AttributeEvent<ValueType>& > >
    class EventInputAttribute : public AInputAttribute<ValueType>
    {
    public:
        EventInputAttribute( const FunctionType& function, bool multipleSources ) :
                AInputAttribute<ValueType>( multipleSources ),
                _function( function )
        {
        }

        virtual ~EventInputAttribute()
        {
        }

    protected:
        virtual void onSourceAttach( IAttribute& output )
        {
            AttributeEvent<ValueType> event( E_SOURCE_ATTACH_EVENT, *this,
                                             this->getSourceAttribute( output ) );
            _function( event );
        }

        virtual void onSourceDetach( IAttribute& output )
        {
            AttributeEvent<ValueType> event( E_SOURCE_DETACH_EVENT, *this,
                                             this->getSourceAttribute( output ) );
            _function( event );
        }

        virtual bool onSourceUpdate( IAttribute& output )
        {
            AttributeEvent<ValueType> event( E_SOURCE_UPDATE_EVENT, *this,
                                             this->getSourceAttribute( output ) );
            return _function( event );
        }

        virtual bool onStringUpdate( const std::string& valueString )
        {
            AttributeEvent<ValueType> event( E_STRING_UPDATE_EVENT, *this, valueString );
            return _function( event );
        }

        FunctionType _function;
    };

    // create function

    template<typename ValueType>
    inline EventInputAttribute<ValueType,Function<bool,const AttributeEvent<ValueType>& > >* createInputAttribute(
        const Function<bool,const AttributeEvent<ValueType>& >& function, bool multipleSources )
    {
        return new EventInputAttribute<ValueType>( function, multipleSources );
    }

    template<typename ValueType, typename FunctionType>
    inline EventInputAttribute<ValueType,FunctionType>* createInputAttribute(
        const FunctionType& function, bool multipleSources )
    {
        return new EventInputAttribute<ValueType,FunctionType>( function, multipleSources );
    }

} // namespace coca

#endif //COCA_EVENT_INPUT_ATTRIBUTE_H_INCLUDED
