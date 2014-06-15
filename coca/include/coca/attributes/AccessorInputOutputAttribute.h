// Copyright (C) 2008-2009 Harm Boschloo

#ifndef COCA_ACCESSOR_INPUT_OUTPUT_ATTRIBUTE_H_INCLUDED
#define COCA_ACCESSOR_INPUT_OUTPUT_ATTRIBUTE_H_INCLUDED


#include "AInputOutputAttribute.h"
#include "../utils/function/Accessor.h"

namespace coca
{
    template<typename ValueType, typename AccessorType = Accessor<ValueType> >
    class AccessorInputOutputAttribute : public AInputOutputAttribute<ValueType>
    {
    public:
        AccessorInputOutputAttribute( const AccessorType& accessor ) :
                AInputOutputAttribute<ValueType>( false ),
                _accessor( accessor )
        {
        }

        virtual ~AccessorInputOutputAttribute()
        {
        }

        virtual ValueType getValue() const
        {
            return _accessor();
        }

    protected:
        virtual void onSourceAttach( IAttribute& output )
        {
            this->doSourceUpdate( output );
        }

        virtual bool onSourceUpdate( IAttribute& output )
        {
            _accessor( this->getSourceAttribute( output ).getValue() );
            return true;
        }

        virtual bool onStringUpdate( const std::string& valueString )
        {
            ValueType value;
            if ( !fromString( valueString, value ) ) { return false; }
            _accessor( value );
            return true;
        }

        AccessorType _accessor;
    };

    // pointer partial specialization

    template<typename ValueType, typename AccessorType >
    class AccessorInputOutputAttribute<ValueType*,AccessorType> : public AInputOutputAttribute<ValueType*>
    {
    public:
        AccessorInputOutputAttribute<ValueType*,AccessorType>( const AccessorType& accessor ) :
                AInputOutputAttribute<ValueType*>( false ),
                _accessor( accessor )
        {
        }

        virtual ~AccessorInputOutputAttribute()
        {
        }

        virtual ValueType* getValue() const
        {
            return _accessor();
        }

    protected:
        virtual void onSourceAttach( IAttribute& output )
        {
            this->doSourceUpdate( output );
        }

        virtual void onSourceDetach( IAttribute& COCA_HIDE( output ) )
        {
            _accessor( 0 );
            if ( this->hasAutoUpdate() ) { this->update(); }
        }

        virtual bool onSourceUpdate( IAttribute& output )
        {
            _accessor( this->getSourceAttribute( output ).getValue() );
            return true;
        }

        AccessorType _accessor;
    };

    // create function //

    template<typename ValueType, typename AccessorType>
    inline AccessorInputOutputAttribute<ValueType,AccessorType>* createInputOutputAttribute(
        const AccessorType& accessor )
    {
        return new AccessorInputOutputAttribute<ValueType,AccessorType>( accessor );
    }

} // namespace coca

#endif //COCA_ACCESSOR_INPUT_OUTPUT_ATTRIBUTE_H_INCLUDED
