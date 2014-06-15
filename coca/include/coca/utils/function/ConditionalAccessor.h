// Copyright (C) 2008-2009 Harm Boschloo

#ifndef COCA_CONDITIONAL_ACCESSOR_H_INCLUDED
#define COCA_CONDITIONAL_ACCESSOR_H_INCLUDED

#include "ConditionalFunctionAccessorAdaptor.h"
#include "../ReferencePointer.h"

namespace coca
{
    template<typename Class, typename DataType>
    class ConditionalAccessor
    {
    public:
        ConditionalAccessor( AConditionalAccessorAdaptor<Class,DataType>* adaptor ) :
                _adaptor( adaptor )
        {
        }

        ConditionalAccessor( const ConditionalAccessor<Class,DataType>& other ) :
                _adaptor( other._adaptor )
        {
        }

        ConditionalAccessor<Class,DataType>& operator=( const ConditionalAccessor<Class,DataType>& other )
        {
            _adaptor = other._adaptor;
            return *this;
        }

        inline bool operator==( const ConditionalAccessor<Class,DataType>& other ) const
        {
            return ( _adaptor == other._adaptor );
        }

        inline bool operator!=( const ConditionalAccessor<Class,DataType>& other ) const
        {
            return !( other==*this );
        }

        inline Class* getObject() const
        {
            return _adaptor->getObject();
        }

        inline void setObject( Class* object )
        {
            _adaptor->setObject( object );
        }

        inline DataType operator()() const
        {
            return ( *_adaptor )();
        }

        inline void operator()( DataType data ) const
        {
            return ( *_adaptor )( data );
        }

        const AConditionalAccessorAdaptor<Class,DataType>* getAdaptor() const
        {
            return _adaptor;
        }

    private:
        ReferencePointer<AConditionalAccessorAdaptor<Class,DataType> > _adaptor;
    };

    // make functions

    template<typename Class, typename DataType, typename GetReturn, typename SetReturn, typename SetArgument, typename Accessor>
    inline ConditionalAccessor<Class,DataType> makeConditionalAccessor(
        Class* object, const Accessor& accessor )
    {
        return ConditionalAccessor<Class,DataType>( createConditionalAccessorAdaptor<Class,DataType>( object, accessor ) );
    }

    template<typename Class, typename DataType, typename GetReturn, typename SetReturn, typename SetArgument>
    inline ConditionalAccessor<Class,DataType> makeConditionalAccessor(
        Class* object, GetReturn( Class::*getter )(), SetReturn( Class::*setter )( SetArgument ) )
    {
        return ConditionalAccessor<Class,DataType>( createConditionalAccessorAdaptor<Class,DataType>( object, getter, setter ) );
    }

    template<typename Class, typename DataType, typename GetReturn, typename SetReturn, typename SetArgument>
    inline ConditionalAccessor<Class,DataType> makeConditionalAccessor(
        Class* object, GetReturn( Class::*getter )() const, SetReturn( Class::*setter )( SetArgument ) )
    {
        return ConditionalAccessor<Class,DataType>( createConditionalAccessorAdaptor<Class,DataType>( object, getter, setter ) );
    }

    template<typename Class, typename DataType, typename GetReturn, typename SetReturn, typename SetArgument>
    inline ConditionalAccessor<Class,DataType> makeConditionalAccessor(
        Class* object, GetReturn( Class::*getter )(), SetReturn( Class::*setter )( SetArgument ) const )
    {
        return ConditionalAccessor<Class,DataType>( createConditionalAccessorAdaptor<Class,DataType>( object, getter, setter ) );
    }

    template<typename Class, typename DataType, typename GetReturn, typename SetReturn, typename SetArgument>
    inline ConditionalAccessor<const Class,DataType> makeConditionalAccessor(
        const Class* object, GetReturn( Class::*getter )() const, SetReturn( Class::*setter )( SetArgument ) const )
    {
        return ConditionalAccessor<Class,DataType>( createConditionalAccessorAdaptor<Class,DataType>( object, getter, setter ) );
    }

} // namespace coca

#endif // COCA_CONDITIONAL_ACCESSOR_H_INCLUDED

