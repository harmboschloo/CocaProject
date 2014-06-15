// Copyright (C) 2008-2009 Harm Boschloo

#ifndef COCA_FILE_H_INCLUDED
#define COCA_FILE_H_INCLUDED

#include <string>

namespace coca
{
    inline std::string extractFilename( const std::string& filePath )
    {
        size_t n = filePath.find_last_of( "/\\" );
        if ( n == std::string::npos ) { return filePath; }
        else { return filePath.substr( n+1 ); }
    }

    inline std::string extractDirectory( const std::string& filePath )
    {
        size_t n = filePath.find_last_of( "/\\" );
        if ( n == std::string::npos ) { return "."; }
        else { return filePath.substr( 0, n ); }
    }

    inline std::string extractExtension( const std::string& filePath )
    {
        size_t n = filePath.find_last_of( "." );
        if ( n == std::string::npos ) { return ""; }
        else { return filePath.substr( n+1 ); }
    }

    inline std::string stripTrailingPathSeperator( const std::string& path )
    {
        if ( path.empty() ) { return path; }

        std::string newPath = path;
        size_t pos = newPath.size()-1;
        while ( newPath[pos] == '/' || newPath[pos] == '\\' )
        {
            newPath.erase( pos, 1 );
            if ( pos == 0 ) { break; }
        }
        return newPath;
    }

} // namespace coca

#endif // COCA_FILE_H_INCLUDED

