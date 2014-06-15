// Copyright (C) 2008-2009 Harm Boschloo

#ifndef COCA_STRING_H_INCLUDED
#define COCA_STRING_H_INCLUDED

#include <string>
#include <sstream>
#include <limits>

namespace coca
{
    static const std::string EMPTY_STRING;

    // algorithms //

    inline std::string& removeCharacters( std::string& str, const std::string& characters )
    {
        size_t pos = str.find_last_of( characters );
        while ( pos != std::string::npos )
        {
            str.erase( pos, 1 );
            if ( pos == 0 ) { break; }
            pos = str.find_last_of( characters, pos-1 );
        }
        return str;
    }

    inline std::string& replaceCharacter( std::string& str, char character, const std::string& replacement )
    {
        size_t pos = str.find_last_of( character );
        while( pos != std::string::npos )
        {
            str.replace( pos, 1, replacement );
            if ( pos == 0 ) { break; }
            pos = str.find_last_of( character, pos-1 );
        }
        return str;
    }

    // toString //

    template<typename T>
    inline std::string toString( const T& value )
    {
        std::ostringstream ss;
        ss.precision( std::numeric_limits<T>::digits10 );
        ss << value;
        return ss.str();
    }

    // std::string specialization of toString
    template<>
    inline std::string toString<std::string>( const std::string& value )
    {
        return value;
    }

    // fromString //

    template<typename T>
    inline bool fromString( const std::string& str, T& value )
    {
        std::istringstream ss(str);
        ss >> value;
        return !ss.fail();
    }

    // std::string specialization of fromString
    template<>
    inline bool fromString<std::string>( const std::string& str, std::string& value )
    {
        value = str;
        return true;
    }

} // namespace coca

#endif // COCA_STRING_H_INCLUDED

