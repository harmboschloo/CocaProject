// Copyright (C) 2008-2009 Harm Boschloo

#ifndef COCA_XML_WRITER_H_INCLUDED
#define COCA_XML_WRITER_H_INCLUDED

#include "../../utils/NonCopyable.h"
#include "../../utils/string.h"
#include <ostream>
#include <string>
#include <stack>
#include <sstream>

namespace coca
{
	class XMLWriter : private NonCopyable
    {
    public:
        XMLWriter( std::ostream& stream ) :
                _stream( stream )
        {
            reset();
        }

        ~XMLWriter()
        {
            _stream.flush();
        }

        void reset( int tabs = 0 )
        {
            _tabs = tabs;
            _tagClosed = true;
            _singleLine = false;
            while ( !_elementNames.empty() ) { _elementNames.pop(); }
        }

        bool singleLine() const
        {
            return _singleLine;
        }

        void setSingleLine( bool singleLine )
        {
            _singleLine = singleLine;
        }

        void closeTag()
        {
            if ( !_tagClosed )
            {
                _stream << ">";
                endLine();
                ++_tabs;
                _tagClosed = true;
            }
        }

        void endLine()
        {
            if ( !_singleLine ) { _stream << std::endl; }
        }

        void writeHeader()
        {
            _stream << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>" << std::endl;
        }

        void writeElementStart( const std::string& name )
        {
            closeTag();
            std::string elementName = insertSpecialCharacters( name );
            _stream << getTabs() << "<" << elementName;
            _elementNames.push( elementName );
            _tagClosed = false;
        }

        void writeAttribute( const std::string& name, const std::string& value )
        {
            if ( !_tagClosed && !name.empty() && !value.empty() )
            {
                _stream << " " << insertSpecialCharacters( name ) << "=\""
                << insertSpecialCharacters( value ) << "\"";
            }
        }

        void writeElementEnd()
        {
            if ( !_tagClosed )
            {
                _stream << "/>" << std::endl;
                _tagClosed = true;
            }
            else
            {
                --_tabs;
                if ( !_singleLine ) { _stream << getTabs(); }
                _stream << "</" << _elementNames.top() << ">" << std::endl;
            }

            _elementNames.pop();
        }

        void writeComment( const std::string& comment, bool doInsertTabs = true )
        {
            std::string commentLocal = insertSpecialCharacters( comment );

            closeTag();
            if ( !_singleLine && doInsertTabs )
            {
                insertTabs( commentLocal );
                _stream << getTabs();
            }
            _stream << "<!--" << commentLocal << "-->";
            endLine();
        }

        void writeText( const std::string& text, bool doInsertTabs = true )
        {
            std::string textLocal = insertSpecialCharacters( text );

            closeTag();

            if ( !_singleLine && doInsertTabs )
            {
                insertTabs( textLocal );
                _stream << getTabs();
            }

            _stream << textLocal;
            endLine();
        }

        template<typename T>
        void writeAttribute( const std::string& name, const T& value )
        {
            std::ostringstream ss;
            ss << value;
            writeAttribute( name, ss.str() );
        }

    protected:
        std::string getTabs() const
        {
            return std::string( _tabs*4, ' ' );
        }

        void insertTabs( std::string& str ) const
        {
            std::string tabs = getTabs();
            size_t pos = str.find_first_of( '\n' );
            while ( pos != std::string::npos )
            {
                ++pos;
                if ( pos < str.size() )
                {
                    str.insert( pos, tabs );
                    pos = str.find_first_of( '\n', ( pos+tabs.size() ) );
                }
            }
        }

        std::string insertSpecialCharacters( const std::string& str )
        {
            std::string newStr( str );
            coca::replaceCharacter( newStr, '&', "&amp;" );
            coca::replaceCharacter( newStr, '<', "&lt;" );
            coca::replaceCharacter( newStr, '>', "&gt;" );
            coca::replaceCharacter( newStr, '"', "&quot;" );
            coca::replaceCharacter( newStr, '\'', "&apos;" );
            return newStr;
        }

        std::ostream& _stream;
        int _tabs;
        bool _tagClosed;
        bool _singleLine;
        std::stack<std::string> _elementNames;

    };

} // namespace coca

#endif // COCA_XML_WRITER_H_INCLUDED
