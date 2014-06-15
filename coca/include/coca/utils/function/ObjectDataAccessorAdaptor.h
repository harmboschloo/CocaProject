// Copyright (C) 2008-2009 Harm Boschloo

#ifndef COCA_OBJECT_DATA_ACCESSOR_ADAPTOR_H_INCLUDED
#define COCA_OBJECT_DATA_ACCESSOR_ADAPTOR_H_INCLUDED

#include "IAccessorAdaptor.h"
#include "ObjectDataAccessor.h"

namespace coca
{
    template<typename Data, typename Class, typename Accessor = ObjectDataAccessor<Data,Class> >
    class ObjectDataAccessorAdaptor : public IAccessorAdaptor<const Data&>
    {
    public:
        ObjectDataAccessorAdaptor( const Accessor& accessor ) :
                _accessor( accessor )
        {
        }

        virtual const Data& operator()() const
        {
            return _accessor();
        }

        virtual void operator()( const Data& data ) const
        {
            _accessor( data );
        }

    private:
        Accessor _accessor;
    };

    // make functions

    template<typename Data, typename Class>
    inline ObjectDataAccessorAdaptor<Data,Class>* createAccessorAdaptor(
        Class& object, Data( Class::*dataMemberPointer ) )
    {
        return new ObjectDataAccessorAdaptor<Data,Class>( makeObjectAccessor( object, dataMemberPointer ) );
    }

} // namespace coca

#endif // COCA_OBJECT_DATA_ACCESSOR_ADAPTOR_H_INCLUDED

