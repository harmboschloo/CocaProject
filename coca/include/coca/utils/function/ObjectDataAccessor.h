// Copyright (C) 2008-2009 Harm Boschloo

#ifndef COCA_OBJECT_DATA_ACCESSOR_H_INCLUDED
#define COCA_OBJECT_DATA_ACCESSOR_H_INCLUDED

#include "RefMemberDataAccessor.h"

namespace coca
{
    template<typename Data, typename Class,
    typename MemberAccessor = RefMemberDataAccessor<Data,Class> >
    class ObjectDataAccessor
    {
    public:
        ObjectDataAccessor( Class& object, const MemberAccessor& accessor ) :
                _object( &object ),
                _accessor( accessor )
        {
        }

        inline const Data& operator()() const
        {
            return _accessor( *_object );
        }

        inline void operator()( const Data& data ) const
        {
            _accessor( *_object, data );
        }

    private:
        Class* _object;
        MemberAccessor _accessor;
    };

    // make functions

    template<typename Data, typename Class>
    inline ObjectDataAccessor<Data,Class> makeObjectAccessor( Class& object, Data( Class::*dataMemberPointer ) )
    {
        return ObjectDataAccessor<Data,Class>( object, makeRefMemberAccessor( dataMemberPointer ) );
    }

} // namespace coca

#endif // COCA_OBJECT_DATA_ACCESSOR_H_INCLUDED

