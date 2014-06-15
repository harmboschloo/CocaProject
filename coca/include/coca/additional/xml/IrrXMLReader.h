// Copyright (C) 2008-2009 Harm Boschloo

#ifndef COCA_IRR_XML_READER_H_INCLUDED
#define COCA_IRR_XML_READER_H_INCLUDED

#include "../../utils/NonCopyable.h"
#include "StreamReadCallback.h"
#include "irr/irrXML_implementation.h"
#include <cstring>

namespace coca
{
	class IrrXMLReader : private NonCopyable
    {
    public:
        IrrXMLReader( std::istream& stream ) :
                _callback( stream ),
                _reader( irr::io::createIrrXMLReader( &_callback ) ),
                _atEmptyElementEnd( false )
        {
        }

        virtual ~IrrXMLReader()
        {
            delete _reader;
        }

        bool atElementStart() const
        {
            return ( !_atEmptyElementEnd && _reader->getNodeType() == irr::io::EXN_ELEMENT );
        }

        bool atElementStart( const std::string& name ) const
        {
            return ( atElementStart() && atNode( name ) );
        }

        bool hasAttribute( const std::string& name ) const
        {
            return ( _reader->getAttributeValue( name.c_str() ) != 0 );
        }

        bool isEmptyElement() const
        {
            return ( atElementStart() && _reader->isEmptyElement() );
        }

        bool atText() const
        {
            return ( _reader->getNodeType() == irr::io::EXN_TEXT );
        }

        bool atElementEnd() const
        {
            return ( _reader->getNodeType() == irr::io::EXN_ELEMENT_END || _atEmptyElementEnd );
        }

        bool atElementEnd( const std::string& name ) const
        {
            return ( atElementEnd() && atNode( name ) );
        }

        bool atNode( const std::string& name ) const
        {
            return ( strcmp( name.c_str(), _reader->getNodeName() ) == 0 );
        }

        std::string getNode() const
        {
            return _reader->getNodeName();
        }

        std::string getText( bool doStripText = true ) const
        {
            std::string str;
            const char* text = _reader->getNodeData();
            if ( text )
            {
                str = text;
                if ( doStripText && str.find_last_of( '\n' ) != std::string::npos ) { str = stripText( str ); }
            }
            return str;
        }

        std::string getAttribute( const std::string& name ) const
        {
            return _reader->getAttributeValueSafe( name.c_str() );
        }

        std::string getAttribute( const std::string& name, const std::string& defaultValue ) const
        {
            if ( hasAttribute( name ) )
            {
                return _reader->getAttributeValueSafe( name.c_str() );
            }
            else { return defaultValue; }
        }

        int getAttributeAsInt( const std::string& name ) const
        {
            return _reader->getAttributeValueAsInt( name.c_str() );
        }

        int getAttributeAsInt( const std::string& name, int defaultValue ) const
        {
            if ( hasAttribute( name ) )
            {
                return _reader->getAttributeValueAsInt( name.c_str() );
            }
            else { return defaultValue; }
        }

        float getAttributeAsFloat( const std::string& name ) const
        {
            return _reader->getAttributeValueAsFloat( name.c_str() );
        }

        float getAttributeAsFloat( const std::string& name, float defaultValue ) const
        {
            if ( hasAttribute( name ) )
            {
                return _reader->getAttributeValueAsFloat( name.c_str() );
            }
            else { return defaultValue; }
       }

        bool read()
        {
            if ( !_atEmptyElementEnd && isEmptyElement() )
            {
                _atEmptyElementEnd = true;
                return true;
            }
            else
            {
                _atEmptyElementEnd = false;
                return _reader->read();
            }
        }

        bool readUntillElementStart( const std::string& name )
        {
            do
            {
                if ( atElementStart( name ) ) { return true; }
            }
            while ( read() );

            return false;
        }

        bool readUntillElementEnd( const std::string& name )
        {
            do
            {
                if ( atElementEnd( name ) ) { return true; }
            }
            while ( read() );

            return false;
        }

        irr::io::IrrXMLReader* getReader() const
        {
            return _reader;
        }

        std::string stripText( const std::string& text ) const
        {
            std::string str( text );

            if ( str.empty() ) { return str; }

            // strip leading stuff
            std::string::iterator it;
            for ( it = str.begin(); ; ++it )
            {
                if ( it == str.end() )
                {
                    str.erase( str.begin(), it );
                    break;
                }

                char c = *it;
                if ( c == '\t' || c == ' ' || c == '\r' || c == '\n' )
                {
                    continue;
                }
                else
                {
                    str.erase( str.begin(), it );
                    break;
                }
            }

            if ( str.empty() ) { return str; }

            // strip other tabs
            std::string::size_type pos = 0;
            while ( pos != str.size() )
            {
                pos = str.find( '\n', pos );

                if ( pos == std::string::npos ) { break; }

                ++pos;
                if ( pos == str.size() ) { break; }

                std::string::size_type begin = pos;
                while ( str[pos] == '\t' || str[pos] == ' ' )
                {
                    ++pos;
                    if ( pos == str.size() ) { break; }
                }
                str.erase( begin, pos-begin );
            }

            if ( str.empty() ) { return str; }

            // strip traling stuff
            it = str.end();
            while ( true )
            {
                --it;

                char c = *it;
                if ( c == '\t' || c == ' ' || c == '\r' || c == '\n' )
                {
                    if ( it == str.begin() ) { break; }
                    else { continue; }
                }
                else
                {
                    ++it;
                    str.erase( it, str.end() );
                    break;
                }

                if ( it == str.begin() ) { break; }
            }

            return str;
        }

    private:
        StreamReadCallback _callback;
        irr::io::IrrXMLReader* _reader;
        bool _atEmptyElementEnd;
    };

} // namespace coca

#endif // COCA_IRR_XML_READER_H_INCLUDED
