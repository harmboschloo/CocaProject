// Copyright (C) 2008-2009 Harm Boschloo

#ifndef COCA_ACCESSOR_H_INCLUDED
#define COCA_ACCESSOR_H_INCLUDED

#include "IAccessorAdaptor.h"
#include "DataAccessorAdaptor.h"
#include "ObjectDataAccessorAdaptor.h"
#include "ObjectFunctionAccessorAdaptor.h"
#include "../ReferencePointer.h"

namespace coca
{
    template<typename GetReturn, typename SetReturn = void, typename SetArgument = GetReturn>
    class Accessor
    {
    public:
        Accessor( IAccessorAdaptor<GetReturn,SetReturn,SetArgument>* adaptor ) :
                _adaptor( adaptor )
        {
        }

        Accessor( const Accessor<GetReturn,SetReturn,SetArgument>& other ) :
                _adaptor( other._adaptor )
        {
        }

        Accessor<GetReturn,SetReturn,SetArgument>& operator=( const Accessor<GetReturn,SetReturn,SetArgument>& other )
        {
            _adaptor = other._adaptor;
            return *this;
        }

        inline bool operator==( const Accessor<GetReturn,SetReturn,SetArgument>& other ) const
        {
            return ( _adaptor == other._adaptor );
        }

        inline bool operator!=( const Accessor<GetReturn,SetReturn,SetArgument>& other ) const
        {
            return !( other==*this );
        }

        inline GetReturn operator()() const
        {
            return ( *_adaptor )();
        }

        inline SetReturn operator()( SetArgument argument ) const
        {
            return ( *_adaptor )( argument );
        }

        const IAccessorAdaptor<GetReturn,SetReturn,SetArgument>* getAdaptor() const
        {
            return _adaptor;
        }

    private:
        ReferencePointer<IAccessorAdaptor<GetReturn,SetReturn,SetArgument> > _adaptor;
    };

    // make functions

    // data accessor
    template<typename Data>
    inline Accessor<const Data&> makeAccessor( Data& data )
    {
        return Accessor<const Data&>( createAccessorAdaptor( data ) );
    }

    // object data accessor
    template<typename Data, typename Class>
    inline Accessor<const Data&> makeAccessor(
        Class& object, Data( Class::*dataMemberPointer ) )
    {
        return Accessor<const Data&>( createAccessorAdaptor( object, dataMemberPointer ) );
    }

    // object function accessors
    template<typename Class, typename GetReturn, typename SetReturn, typename SetArgument>
    inline Accessor<GetReturn,SetReturn,SetArgument> makeAccessor(
        Class& object, GetReturn( Class::*getter )(), SetReturn( Class::*setter )( SetArgument ) )
    {
        return Accessor<GetReturn,SetReturn,SetArgument>( createAccessorAdaptor( object, getter, setter ) );
    }

    template<typename Class, typename GetReturn, typename SetReturn, typename SetArgument>
    inline Accessor<GetReturn,SetReturn,SetArgument> makeAccessor(
        Class& object, GetReturn( Class::*getter )() const, SetReturn( Class::*setter )( SetArgument ) )
    {
        return Accessor<GetReturn,SetReturn,SetArgument>( createAccessorAdaptor( object, getter, setter ) );
    }

    template<typename Class, typename GetReturn, typename SetReturn, typename SetArgument>
    inline Accessor<GetReturn,SetReturn,SetArgument> makeAccessor(
        Class& object, GetReturn( Class::*getter )(), SetReturn( Class::*setter )( SetArgument ) const )
    {
        return Accessor<GetReturn,SetReturn,SetArgument>( createAccessorAdaptor( object, getter, setter ) );
    }

    template<typename Class, typename GetReturn, typename SetReturn, typename SetArgument>
    inline Accessor<GetReturn,SetReturn,SetArgument> makeAccessor(
        Class& object, GetReturn( Class::*getter )() const, SetReturn( Class::*setter )( SetArgument ) const )
    {
        return Accessor<GetReturn,SetReturn,SetArgument>( createAccessorAdaptor( object, getter, setter ) );
    }

} // namespace coca

#endif // COCA_ACCESSOR_H_INCLUDED

