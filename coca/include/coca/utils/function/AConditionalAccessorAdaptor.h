// Copyright (C) 2008-2009 Harm Boschloo

#ifndef COCA_A_CONDITIONAL_ACCESSOR_ADAPTOR_H_INCLUDED
#define COCA_A_CONDITIONAL_ACCESSOR_ADAPTOR_H_INCLUDED

#include "../ReferenceCounted.h"

namespace coca
{
    template<typename Class, typename DataType>
    class AConditionalAccessorAdaptor : public ReferenceCounted
    {
    public:
        AConditionalAccessorAdaptor( Class* object = 0 ) :
                _data(),
                _object( 0 )
        {
            setObject( object );
        }

        AConditionalAccessorAdaptor( const DataType& defaultValue, Class* object ) :
                _data( defaultValue ),
                _object( 0 )
        {
            setObject( object );
        }

        inline DataType operator()()
        {
            if ( _object ) { _data = getData( *_object ); }
            return _data;
        }

        inline void operator()( DataType data )
        {
            _data = data;
            if ( _object ) { setData( *_object, _data ); }
        }

        Class* getObject() const
        {
            return _object;
        }

        void setObject( Class* object )
        {
            if ( _object == object ) { return; }
            if ( _object ) { _data = getData( *_object ); }
            _object  = object;
            if ( _object ) { setData( *_object, _data ); }
        }

        virtual DataType getData( Class& object ) = 0;
        virtual void setData( Class& object, DataType data ) = 0;

    protected:
        DataType _data;
        Class* _object;
    };

} // namespace coca

#endif // COCA_A_CONDITIONAL_ACCESSOR_ADAPTOR_H_INCLUDED

