// Copyright (C) 2008-2009 Harm Boschloo

#ifndef COCA_OBJECT_FUNCTION_ACCESSOR_ADAPTOR_H_INCLUDED
#define COCA_OBJECT_FUNCTION_ACCESSOR_ADAPTOR_H_INCLUDED

#include "IAccessorAdaptor.h"
#include "ObjectFunctionAccessor.h"

namespace coca
{
    template<typename Class, typename GetReturn, typename GetClass = const Class,
    typename SetReturn = void, typename SetClass = Class, typename SetArgument = GetReturn,
    typename Accessor = ObjectFunctionAccessor<Class,GetReturn,GetClass,SetReturn,SetClass,SetArgument> >
    class ObjectFunctionAccessorAdaptor : public IAccessorAdaptor<GetReturn,SetReturn,SetArgument>
    {
    public:
        ObjectFunctionAccessorAdaptor( const Accessor& accessor ) :
                _accessor( accessor )
        {
        }

        virtual GetReturn operator()() const
        {
            return _accessor();
        }

        virtual SetReturn operator()( SetArgument data ) const
        {
            return _accessor( data );
        }

    private:
        Accessor _accessor;
    };

    // make functions

    template<typename Class, typename GetReturn, typename SetReturn, typename SetArgument, typename Accessor>
    inline ObjectFunctionAccessorAdaptor<Class,GetReturn,const Class,SetReturn,Class,SetArgument,Accessor>* createAccessorAdaptor(
        const Accessor& accessor )
    {
        return new ObjectFunctionAccessorAdaptor<Class,GetReturn,const Class,SetReturn,Class,SetArgument,Accessor>( accessor );
    }

    template<typename Class, typename GetReturn, typename SetReturn, typename SetArgument>
    inline ObjectFunctionAccessorAdaptor<Class,GetReturn,Class,SetReturn,Class,SetArgument>* createAccessorAdaptor(
        Class& object, GetReturn( Class::*getter )(), SetReturn( Class::*setter )( SetArgument ) )
    {
        return new ObjectFunctionAccessorAdaptor<Class,GetReturn,Class,SetReturn,Class,SetArgument>(
                   makeObjectAccessor( object, getter, setter ) );
    }

    template<typename Class, typename GetReturn, typename SetReturn, typename SetArgument>
    inline ObjectFunctionAccessorAdaptor<Class,GetReturn,const Class,SetReturn,Class,SetArgument>* createAccessorAdaptor(
        Class& object, GetReturn( Class::*getter )() const, SetReturn( Class::*setter )( SetArgument ) )
    {
        return new ObjectFunctionAccessorAdaptor<Class,GetReturn,const Class,SetReturn,Class,SetArgument>(
                   makeObjectAccessor( object, getter, setter ) );
    }

    template<typename Class, typename GetReturn, typename SetReturn, typename SetArgument>
    inline ObjectFunctionAccessorAdaptor<Class,GetReturn,Class,SetReturn,const Class,SetArgument>* createAccessorAdaptor(
        Class& object, GetReturn( Class::*getter )(), SetReturn( Class::*setter )( SetArgument ) const )
    {
        return new ObjectFunctionAccessorAdaptor<Class,GetReturn,Class,SetReturn,const Class,SetArgument>(
                   makeObjectAccessor( object, getter, setter ) );
    }

    template<typename Class, typename GetReturn, typename SetReturn, typename SetArgument>
    inline ObjectFunctionAccessorAdaptor<const Class,GetReturn,const Class,SetReturn,const Class,SetArgument>* createAccessorAdaptor(
        const Class& object, GetReturn( Class::*getter )() const, SetReturn( Class::*setter )( SetArgument ) const )
    {
        return new ObjectFunctionAccessorAdaptor<const Class,GetReturn,const Class,SetReturn,const Class,SetArgument>(
                   makeObjectAccessor( object, getter, setter ) );
    }

} // namespace coca

#endif // COCA_OBJECT_FUNCTION_ACCESSOR_ADAPTOR_H_INCLUDED

