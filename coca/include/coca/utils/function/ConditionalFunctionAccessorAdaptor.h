// Copyright (C) 2008-2009 Harm Boschloo

#ifndef COCA_CONDITIONAL_FUNCTION_ACCESSOR_ADAPTOR_H_INCLUDED
#define COCA_CONDITIONAL_FUNCTION_ACCESSOR_ADAPTOR_H_INCLUDED

#include "AConditionalAccessorAdaptor.h"
#include "RefMemberFunctionAccessor.h"

namespace coca
{
    template<typename Class, typename DataType,
    typename GetReturn = DataType, typename GetClass = const Class,
    typename SetReturn = void, typename SetClass = Class, typename SetArgument = DataType,
    typename Accessor = RefMemberFunctionAccessor<GetReturn,GetClass,SetReturn,SetClass,SetArgument> >
    class ConditionalFunctionAccessorAdaptor : public AConditionalAccessorAdaptor<Class,DataType>
    {
    public:
        ConditionalFunctionAccessorAdaptor( const Accessor& accessor, Class* object = 0 ) :
                AConditionalAccessorAdaptor<Class,DataType>( object ),
                _accessor( accessor )
        {
        }

        ConditionalFunctionAccessorAdaptor( const Accessor& accessor, const DataType& defaultValue,
                                           Class* object ) :
                AConditionalAccessorAdaptor<Class,DataType>( defaultValue, object ),
                _accessor( accessor )
        {
        }

        DataType getData( Class& object )
        {
            return _accessor( object );
        }

        void setData( Class& object, DataType data )
        {
            _accessor( object, data );
        }

    private:
        Accessor _accessor;
    };

    // make functions

    template<typename Class, typename DataType, typename GetReturn, typename SetReturn, typename SetArgument, typename Accessor>
    inline ConditionalFunctionAccessorAdaptor<Class,DataType,GetReturn,const Class,SetReturn,Class,SetArgument,Accessor>* createConditionalAccessorAdaptor(
        Class* object, const Accessor& accessor )
    {
        return new ConditionalFunctionAccessorAdaptor<Class,DataType,GetReturn,const Class,SetReturn,Class,SetArgument,Accessor>(
                   accessor, object );
    }

    template<typename Class, typename DataType, typename GetReturn, typename SetReturn, typename SetArgument>
    inline ConditionalFunctionAccessorAdaptor<Class,DataType,GetReturn,Class,SetReturn,Class,SetArgument>* createConditionalAccessorAdaptor(
        Class* object, GetReturn( Class::*getter )(), SetReturn( Class::*setter )( SetArgument ) )
    {
        return new ConditionalFunctionAccessorAdaptor<Class,DataType,GetReturn,Class,SetReturn,Class,SetArgument>(
                   makeRefMemberAccessor( getter, setter ), object );
    }

    template<typename Class, typename DataType, typename GetReturn, typename SetReturn, typename SetArgument>
    inline ConditionalFunctionAccessorAdaptor<Class,DataType,GetReturn,const Class,SetReturn,Class,SetArgument>* createConditionalAccessorAdaptor(
        Class*object, GetReturn( Class::*getter )() const, SetReturn( Class::*setter )( SetArgument ) )
    {
        return new ConditionalFunctionAccessorAdaptor<Class,DataType,GetReturn,const Class,SetReturn,Class,SetArgument>(
                   makeRefMemberAccessor( getter, setter ), object );
    }

    template<typename Class, typename DataType, typename GetReturn, typename SetReturn, typename SetArgument>
    inline ConditionalFunctionAccessorAdaptor<Class,DataType,GetReturn,Class,SetReturn,const Class,SetArgument>* createConditionalAccessorAdaptor(
        Class* object, GetReturn( Class::*getter )(), SetReturn( Class::*setter )( SetArgument ) const )
    {
        return new ConditionalFunctionAccessorAdaptor<Class,DataType,GetReturn,Class,SetReturn,const Class,SetArgument>(
                   makeRefMemberAccessor( getter, setter ), object );
    }

    template<typename Class, typename DataType, typename GetReturn, typename SetReturn, typename SetArgument>
    inline ConditionalFunctionAccessorAdaptor<const Class,DataType,GetReturn,const Class,SetReturn,const Class,SetArgument>* createConditionalAccessorAdaptor(
        const Class* object, GetReturn( Class::*getter )() const, SetReturn( Class::*setter )( SetArgument ) const )
    {
        return new ConditionalFunctionAccessorAdaptor<const Class,DataType,GetReturn,const Class,SetReturn,const Class,SetArgument>(
                   makeRefMemberAccessor( getter, setter ), object );
    }

} // namespace coca

#endif // COCA_CONDITIONAL_FUNCTION_ACCESSOR_ADAPTOR_H_INCLUDED

