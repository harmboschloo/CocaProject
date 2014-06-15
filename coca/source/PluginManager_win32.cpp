// Copyright (C) 2007-2008 Harm Boschloo

#include <coca/utils/config.h>

#if defined(COCA_WIN32)

#include "PluginManager.h"
#include <coca/utils/file.h>
#include <windows.h>

namespace coca
{
    std::string PluginManager::getDefaultPluginDirectory()
    {
        HMODULE handle = GetModuleHandle( "coca" );
        if ( !handle )
        {
            COCA_ERROR( "Could not get module handle for the coca library" );
            return "";
        }

        char path[MAX_PATH];
        if ( GetModuleFileName( handle, path, MAX_PATH ) == 0 )
        {
            COCA_ERROR( "Could not get module file name for the coca library" );
            return "";
        }

        return extractDirectory( path ).append( "\\plugins" );
    }

    int PluginManager::loadPlugins( const std::string& directory )
    {
        std::string dir;
        if ( directory.empty() ) { dir = "."; }
        else { dir = stripTrailingPathSeperator( directory ); }

        std::string fileFilter( dir + "\\*.dll" );
        WIN32_FIND_DATA findData;
        HANDLE findHandle = ::FindFirstFile( fileFilter.c_str(), &findData );

        if ( findHandle == INVALID_HANDLE_VALUE ) { return 0; }

        int count = 0;

        do
        {
            if ( findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY ) { continue; }
            std::string pluginFilename = dir + "\\" + findData.cFileName;
            IPluginData* data = loadPlugin( pluginFilename );
            if ( data ) { ++count; }
        }
        while ( ::FindNextFile( findHandle, &findData ) );

        return count;
    }

} // namespace coca

#endif
