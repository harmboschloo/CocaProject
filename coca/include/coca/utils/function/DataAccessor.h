// Copyright (C) 2008-2009 Harm Boschloo

#ifndef COCA_DATA_ACCESSOR_H_INCLUDED
#define COCA_DATA_ACCESSOR_H_INCLUDED

namespace coca
{
    template<typename Data>
    class DataAccessor
    {
    public:
        DataAccessor( Data& data ) :
                _data( &data )
        {
        }

        inline const Data& operator()() const
        {
            return *_data;
        }

        inline void operator()( const Data& data ) const
        {
            *_data = data;
        }

    private:
        Data* _data;
    };

    // make functions

    template<typename Data>
    inline DataAccessor<Data> makeDataAccessor( Data& data )
    {
        return DataAccessor<Data>( data );
    }

} // namespace coca

#endif // COCA_DATA_ACCESSOR_H_INCLUDED

