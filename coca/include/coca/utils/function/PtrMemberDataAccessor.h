// Copyright (C) 2008-2009 Harm Boschloo

#ifndef COCA_PTR_MEMBER_DATA_ACCESSOR_H_INCLUDED
#define COCA_PTR_MEMBER_DATA_ACCESSOR_H_INCLUDED

namespace coca
{
    template<typename Data, typename Class>
    class PtrMemberDataAccessor
    {
    public:
        typedef Data( Class::*DataMemberPointer );

        PtrMemberDataAccessor( DataMemberPointer dataMemberPointer ) :
                _dataMemberPointer( dataMemberPointer )
        {
        }

        inline const Data& operator()( const Class* object ) const
        {
            return ( object->*_dataMemberPointer );
        }

        inline void operator()( Class* object, const Data& data ) const
        {
            ( object->*_dataMemberPointer ) = data;
        }

    private:
        DataMemberPointer _dataMemberPointer;
    };

    template<typename Data, typename Class>
    inline PtrMemberDataAccessor<Data,Class> makePtrMemberAccessor(
        Data( Class::*dataMemberPointer ) )
    {
        return PtrMemberDataAccessor<Data,Class>( dataMemberPointer );
    }

} // namespace coca

#endif // COCA_PTR_MEMBER_DATA_ACCESSOR_H_INCLUDED

