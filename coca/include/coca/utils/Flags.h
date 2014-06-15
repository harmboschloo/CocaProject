// Copyright (C) 2008-2009 Harm Boschloo

#ifndef COCA_FLAGS_H_INCLUDED
#define COCA_FLAGS_H_INCLUDED

namespace coca
{
    template<typename DataType>
    class Flags
    {
    public:
        Flags( DataType flags = 0 ) : _flags( flags )
        {
        }

        ~Flags()
        {
        }

        void set( DataType flags )
        {
            _flags |= flags;
        }

        void set( DataType flags, bool enabled )
        {
            if ( enabled ) { set( flags ); }
            else { unset( flags ); }
        }

        void unset( DataType flags )
        {
            _flags &= ~flags;
        }

        bool isSet( DataType flag ) const
        {
            return ( ( _flags&flag ) != 0 );
        }

        bool areSet( DataType flags ) const
        {
            return ( ( _flags|flags ) == _flags );
        }

        void reset( DataType flags )
        {
            _flags = flags;
        }

        DataType get() const
        {
            return _flags;
        }

    private:
        DataType _flags;
    };

} // namespace coca

#endif //COCA_FLAGS_H_INCLUDED
