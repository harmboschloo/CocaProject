// Copyright (C) 2008-2009 Harm Boschloo

#ifndef COCA_REF_MEMBER_FUNCTION_ACCESSOR_H_INCLUDED
#define COCA_REF_MEMBER_FUNCTION_ACCESSOR_H_INCLUDED

#include "autogen/RefMemberFunction0.h"
#include "autogen/RefMemberFunction1.h"

namespace coca
{
    // NOTE: Inheriting from Getter & Setter does not work. Compilers can't determine
    // which operator to call (RefMemberFunction0 or RefMemberFunction1).
    // Probably because they are of the same class RefMemberFunction.

    template<typename GetReturn, typename GetClass,
    typename SetReturn = void, typename SetClass = GetClass, typename SetArgument = GetReturn,
    typename Getter = RefMemberFunction<GetReturn,GetClass>,
    typename Setter = RefMemberFunction<SetReturn,SetClass,SetArgument> >
    class RefMemberFunctionAccessor
    {
    public:
        RefMemberFunctionAccessor( Getter getter, Setter setter ) :
                _getter( getter ),
                _setter( setter )
        {
        }

        inline GetReturn operator()( GetClass& object ) const
        {
            return _getter( object );
        }

        inline SetReturn operator()( SetClass& object, SetArgument argument ) const
        {
            return _setter( object, argument );
        }

    private:
        Getter _getter;
        Setter _setter;
    };

    template<typename GetReturn, typename GetClass, typename SetReturn, typename SetClass, typename SetArgument>
    inline RefMemberFunctionAccessor<GetReturn,GetClass,SetReturn,SetClass,SetArgument> makeRefMemberAccessor(
        GetReturn( GetClass::*getter )(), SetReturn( SetClass::*setter )( SetArgument ) )
    {
        return RefMemberFunctionAccessor<GetReturn,GetClass,SetReturn,SetClass,SetArgument>(
                   makeRefMemberFunction( getter ), makeRefMemberFunction( setter ) );
    }

    template<typename GetReturn, typename GetClass, typename SetReturn, typename SetClass, typename SetArgument>
    inline RefMemberFunctionAccessor<GetReturn,const GetClass,SetReturn,SetClass,SetArgument> makeRefMemberAccessor(
        GetReturn( GetClass::*getter )() const, SetReturn( SetClass::*setter )( SetArgument ) )
    {
        return RefMemberFunctionAccessor<GetReturn,const GetClass,SetReturn,SetClass,SetArgument>(
                   makeRefMemberFunction( getter ), makeRefMemberFunction( setter ) );
    }

    template<typename GetReturn, typename GetClass, typename SetReturn, typename SetClass, typename SetArgument>
    inline RefMemberFunctionAccessor<GetReturn,GetClass,SetReturn,const SetClass,SetArgument> makeRefMemberAccessor(
        GetReturn( GetClass::*getter )(), SetReturn( SetClass::*setter )( SetArgument ) const )
    {
        return RefMemberFunctionAccessor<GetReturn,GetClass,SetReturn,const SetClass,SetArgument>(
                   makeRefMemberFunction( getter ), makeRefMemberFunction( setter ) );
    }

    template<typename GetReturn, typename GetClass, typename SetReturn, typename SetClass, typename SetArgument>
    inline RefMemberFunctionAccessor<GetReturn,const GetClass,SetReturn,const SetClass,SetArgument> makeRefMemberAccessor(
        GetReturn( GetClass::*getter )() const, SetReturn( SetClass::*setter )( SetArgument ) const )
    {
        return RefMemberFunctionAccessor<GetReturn,const GetClass,SetReturn,const SetClass,SetArgument>(
                   makeRefMemberFunction( getter ), makeRefMemberFunction( setter ) );
    }

} // namespace coca

#endif // COCA_REF_MEMBER_FUNCTION_ACCESSOR_H_INCLUDED

