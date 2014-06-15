// Copyright (C) 2008-2009 Harm Boschloo

#ifndef COCA_STREAM_READ_CALLBACK_H_INCLUDED
#define COCA_STREAM_READ_CALLBACK_H_INCLUDED

#include "irr/irrXML.h"
#include "../../utils/config.h"
#include <istream>

namespace coca
{
    class StreamReadCallback : public irr::io::IFileReadCallBack
    {
    public:
        StreamReadCallback( std::istream& stream ) :
                _stream( &stream )
        {
            _stream->seekg( 0, std::ios::end );
            _size = _stream->tellg();
            _stream->seekg( 0, std::ios::beg );
        }

        ~StreamReadCallback()
        {
        }

        //! Reads an amount of bytes from the file.
        /** \param buffer: Pointer to buffer where to read bytes will be written to.
        \param sizeToRead: Amount of bytes to read from the file.
        \return Returns how much bytes were read. */
        int read( void* buffer, int sizeToRead )
        {
            _stream->read( reinterpret_cast<char*>( buffer ), sizeToRead );
            return _stream->gcount();
        }

        //! Returns size of file in bytes
        int getSize()
        {
            return _size;
        }

    private:
        size_t _size;
        std::istream* _stream;
    };

} // namespace coca

#endif // COCA_STREAM_READ_CALLBACK_H_INCLUDED
