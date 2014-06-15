// Copyright (C) 2008-2009 Harm Boschloo

#ifndef COCA_PTR_MEMBER_FUNCTION_ACCESSOR_H_INCLUDED
#define COCA_PTR_MEMBER_FUNCTION_ACCESSOR_H_INCLUDED

#include "autogen/PtrMemberFunction0.h"
#include "autogen/PtrMemberFunction1.h"

namespace coca
{
    // NOTE: Inheriting from Getter & Setter does not work. Compilers can't determine
    // which operator to call (PtrMemberFunction0 or PtrMemberFunction1).
    // Probably because they are of the same class PtrMemberFunction.

    template<typename GetReturn, typename GetClass,
    typename SetReturn = void, typename SetClass = GetClass, typename SetArgument = GetReturn,
    typename Getter = PtrMemberFunction<GetReturn,GetClass>,
    typename Setter = PtrMemberFunction<SetReturn,SetClass,SetArgument> >
    class PtrMemberFunctionAccessor
    {
    public:
        PtrMemberFunctionAccessor( Getter getter, Setter setter ) :
                _getter( getter ),
                _setter( setter )
        {
        }

        inline GetReturn operator()( GetClass* object ) const
        {
            return _getter( object );
        }

        inline SetReturn operator()( SetClass* object, SetArgument argument ) const
        {
            return _setter( object, argument );
        }

    private:
        Getter _getter;
        Setter _setter;
    };

    template<typename GetReturn, typename GetClass, typename SetReturn, typename SetClass, typename SetArgument>
    inline PtrMemberFunctionAccessor<GetReturn,GetClass,SetReturn,SetClass,SetArgument> makePtrMemberAccessor(
        GetReturn( GetClass::*getter )(), SetReturn( SetClass::*setter )( SetArgument ) )
    {
        return PtrMemberFunctionAccessor<GetReturn,GetClass,SetReturn,SetClass,SetArgument>(
                   makePtrMemberFunction( getter ), makePtrMemberFunction( setter ) );
    }

    template<typename GetReturn, typename GetClass, typename SetReturn, typename SetClass, typename SetArgument>
    inline PtrMemberFunctionAccessor<GetReturn,const GetClass,SetReturn,SetClass,SetArgument> makePtrMemberAccessor(
        GetReturn( GetClass::*getter )() const, SetReturn( SetClass::*setter )( SetArgument ) )
    {
        return PtrMemberFunctionAccessor<GetReturn,const GetClass,SetReturn,SetClass,SetArgument>(
                   makePtrMemberFunction( getter ), makePtrMemberFunction( setter ) );
    }

    template<typename GetReturn, typename GetClass, typename SetReturn, typename SetClass, typename SetArgument>
    inline PtrMemberFunctionAccessor<GetReturn,GetClass,SetReturn,const SetClass,SetArgument> makePtrMemberAccessor(
        GetReturn( GetClass::*getter )(), SetReturn( SetClass::*setter )( SetArgument ) const )
    {
        return PtrMemberFunctionAccessor<GetReturn,GetClass,SetReturn,const SetClass,SetArgument>(
                   makePtrMemberFunction( getter ), makePtrMemberFunction( setter ) );
    }

    template<typename GetReturn, typename GetClass, typename SetReturn, typename SetClass, typename SetArgument>
    inline PtrMemberFunctionAccessor<GetReturn,const GetClass,SetReturn,const SetClass,SetArgument> makePtrMemberAccessor(
        GetReturn( GetClass::*getter )() const, SetReturn( SetClass::*setter )( SetArgument ) const )
    {
        return PtrMemberFunctionAccessor<GetReturn,const GetClass,SetReturn,const SetClass,SetArgument>(
                   makePtrMemberFunction( getter ), makePtrMemberFunction( setter ) );
    }

} // namespace coca

#endif // COCA_PTR_MEMBER_FUNCTION_ACCESSOR_H_INCLUDED

