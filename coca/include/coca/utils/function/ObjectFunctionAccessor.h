// Copyright (C) 2008-2009 Harm Boschloo

#ifndef COCA_OBJECT_FUNCTION_ACCESSOR_H_INCLUDED
#define COCA_OBJECT_FUNCTION_ACCESSOR_H_INCLUDED

#include "RefMemberFunctionAccessor.h"

namespace coca
{
    template<typename Class, typename GetReturn, typename GetClass = const Class,
    typename SetReturn = void, typename SetClass = Class, typename SetArgument = GetReturn,
    typename Accessor = RefMemberFunctionAccessor<GetReturn,GetClass,SetReturn,SetClass,SetArgument> >
    class ObjectFunctionAccessor
    {
    public:
        ObjectFunctionAccessor( Class& object, const Accessor& accessor ) :
                _object( &object ),
                _accessor( accessor )
        {
        }

        inline GetReturn operator()() const
        {
            return _accessor( *_object );
        }

        inline SetReturn operator()( SetArgument data ) const
        {
            return _accessor( *_object, data );
        }

    private:
        Class* _object;
        Accessor _accessor;
    };

    // make functions

    template<typename Class, typename GetReturn, typename SetReturn, typename SetArgument, typename Accessor>
    inline ObjectFunctionAccessor<Class,GetReturn,const Class,SetReturn,Class,SetArgument,Accessor> makeObjectAccessor(
        Class& object, const Accessor& accessor )
    {
        return ObjectFunctionAccessor<Class,GetReturn,const Class,SetReturn,Class,SetArgument,Accessor>(
                   object, accessor );
    }

    template<typename Class, typename GetReturn, typename SetReturn, typename SetArgument>
    inline ObjectFunctionAccessor<Class,GetReturn,Class,SetReturn,Class,SetArgument> makeObjectAccessor(
        Class& object, GetReturn( Class::*getter )(), SetReturn( Class::*setter )( SetArgument ) )
    {
        return ObjectFunctionAccessor<Class,GetReturn,Class,SetReturn,Class,SetArgument>(
                   object, makeRefMemberAccessor( getter, setter ) );
    }

    template<typename Class, typename GetReturn, typename SetReturn, typename SetArgument>
    inline ObjectFunctionAccessor<Class,GetReturn,const Class,SetReturn,Class,SetArgument> makeObjectAccessor(
        Class& object, GetReturn( Class::*getter )() const, SetReturn( Class::*setter )( SetArgument ) )
    {
        return ObjectFunctionAccessor<Class,GetReturn,const Class,SetReturn,Class,SetArgument>(
                   object, makeRefMemberAccessor( getter, setter ) );
    }

    template<typename Class, typename GetReturn, typename SetReturn, typename SetArgument>
    inline ObjectFunctionAccessor<Class,GetReturn,Class,SetReturn,const Class,SetArgument> makeObjectAccessor(
        Class& object, GetReturn( Class::*getter )(), SetReturn( Class::*setter )( SetArgument ) const )
    {
        return ObjectFunctionAccessor<Class,GetReturn,Class,SetReturn,const Class,SetArgument>(
                   object, makeRefMemberAccessor( getter, setter ) );
    }

    template<typename Class, typename GetReturn, typename SetReturn, typename SetArgument>
    inline ObjectFunctionAccessor<const Class,GetReturn,const Class,SetReturn,const Class,SetArgument> makeObjectAccessor(
        const Class& object, GetReturn( Class::*getter )() const, SetReturn( Class::*setter )( SetArgument ) const )
    {
        return ObjectFunctionAccessor<const Class,GetReturn,const Class,SetReturn,const Class,SetArgument>(
                   object, makeRefMemberAccessor( getter, setter ) );
    }

} // namespace coca

#endif // COCA_OBJECT_FUNCTION_ACCESSOR_H_INCLUDED

