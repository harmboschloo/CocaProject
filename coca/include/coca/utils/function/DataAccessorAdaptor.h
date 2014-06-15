// Copyright (C) 2008-2009 Harm Boschloo

#ifndef COCA_DATA_ACCESSOR_ADAPTOR_H_INCLUDED
#define COCA_DATA_ACCESSOR_ADAPTOR_H_INCLUDED

#include "IAccessorAdaptor.h"
#include "DataAccessor.h"

namespace coca
{
    template<typename Data, typename Accessor = DataAccessor<Data> >
    class DataAccessorAdaptor : public IAccessorAdaptor<const Data&>
    {
    public:
        DataAccessorAdaptor( const Accessor& accessor ) : _accessor( accessor ) {}

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

    template<typename Data>
    inline DataAccessorAdaptor<Data>* createAccessorAdaptor( Data& data )
    {
        return new DataAccessorAdaptor<Data>( makeDataAccessor( data ) );
    }

} // namespace coca

#endif // COCA_DATA_ACCESSOR_ADAPTOR_H_INCLUDED

