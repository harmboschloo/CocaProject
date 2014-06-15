// Copyright (C) 2008-2009 Harm Boschloo

#include "Library.h"
#include <coca/utils/config.h>

#if defined(COCA_WIN32)

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

namespace coca
{
    struct LibraryData
    {
        LibraryData() : handle( 0 ) {}
        HMODULE handle;
        std::string filename;
    };

    Library::Library() : _data( new LibraryData() )
    {
    }

    Library::~Library()
    {
        unload();
        delete _data;
    }

    bool Library::load( const std::string& filename )
    {
        unload();
        _data->filename = filename;
        //::SetErrorMode( 0 ); // show dialog
        _data->handle = ::LoadLibrary( _data->filename.c_str() );
        return isLoaded();
    }

    void Library::unload()
    {
        if ( !isLoaded() ) { return; }
        ::FreeLibrary( _data->handle );
        _data->handle = 0;
    }

    bool Library::isLoaded() const
    {
        return ( _data->handle != 0 );
    }

    const std::string& Library::getFilename() const
    {
        return _data->filename;
    }

    void* Library::findSymbol( const std::string& symbolName ) const
    {
        // NOTE: FARPROC or int (*)() cast to void* !
        return ( void* )( GetProcAddress( _data->handle, symbolName.c_str() ) );
    }

} // namespace coca

#endif // COCA_WIN32
