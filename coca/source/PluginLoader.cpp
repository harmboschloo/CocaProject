// Copyright (C) 2007-2008 Harm Boschloo

#include "PluginLoader.h"
#include <coca/IInfo.h>
#include <coca/plugin/APlugin.h>
#include <coca/log/log.h>
#include <memory>

namespace coca
{
    const std::string PluginLoader::CREATE_FUNCTION_NAME( COCA_TO_STRING( COCA_PLUGIN_CREATE_FUNCTION_NAME ) );

    PluginLoader::PluginLoader()
    {
    }

    PluginLoader::~PluginLoader()
    {
    }

    IPlugin* PluginLoader::loadPlugin( void* createSymbol )
    {
        // NOTE: void* cast to APlugin* (*)() !
        CreateFunctionPointer createFunction = ( CreateFunctionPointer )( createSymbol );
        std::auto_ptr<APlugin> plugin(( *createFunction )() );
        if ( !plugin.get() ) { return 0; }

        // check configuration
        const IConfiguration& pluginConfiguration = plugin->getInfo().getConfiguration();
        const IConfiguration& cocaConfiguration = getInfo().getConfiguration();

        if ( pluginConfiguration != cocaConfiguration )
        {
            COCA_ERROR( "Plugin and framework configuration are not compatible!"
                        << " Plugin configuration: platform " << pluginConfiguration.getPlatform()
                        << ", compiler " << pluginConfiguration.getCompiler()
                        << " Framework configuration: platform " << cocaConfiguration.getPlatform()
                        << ", compiler " << cocaConfiguration.getCompiler() << "." );
            return 0;
        }

        // check build version
        const IVersion& pluginCocaVersion = plugin->getInfo().getCocaVersion();
        const IVersion& cocaVersion = getInfo().getVersion();
        if ( pluginCocaVersion.getMajor() != cocaVersion.getMajor() ||
                pluginCocaVersion.getMinor() != cocaVersion.getMinor() )
        {
            COCA_ERROR( "Plugin framework version and framework version are not compatible!"
                        << " Plugin framework version: " << pluginCocaVersion << "."
                        << " Framework version: " << cocaVersion << "." );
            return 0;
        }

        // initialize plugin
        if ( !plugin->init() )
        {
            COCA_ERROR( "Could not initialize plugin." );
            return 0;
        }

        // release plugin
        return plugin.release();
    }

} // namespace coca

