// Copyright (C) 2007-2008 Harm Boschloo

#include "PluginManager.h"
#include "Library.h"
#include "PluginData.h"
#include <coca/plugin/IPlugin.h>
#include <coca/plugin/IPluginLoader.h>
#include <coca/utils/algorithm/eraseItem.h>
#include <memory>

namespace coca
{
    PluginManager::PluginManager()
    {
    }

    PluginManager::~PluginManager()
    {
    }

    bool PluginManager::registerLoader( const std::string& createFunctionName, LoaderRegistry::Creator creator )
    {
        if ( !_loaderRegistry.add( createFunctionName, creator ) )
        {
            COCA_ERROR( "A plugin loader for id '" << createFunctionName << "' already exists!" );
            return false;
        }
        return true;
    }

    const IPluginManager::LoaderRegistry& PluginManager::getLoaderRegistry() const
    {
        return _loaderRegistry;
    }

    int PluginManager::loadPlugins()
    {
        return loadPlugins( getDefaultPluginDirectory() );
    }

    IPluginData* PluginManager::loadPlugin( const std::string& filename )
    {
        COCA_DEBUG_INFO( "PluginManager::loadPlugin: " << filename );

        std::auto_ptr<ILibrary> library( new Library() );
        if ( !library->load( filename ) )
        {
            COCA_ERROR( "Could not load library from filename '" << filename << "'!" );
            return 0;
        }

        const LoaderRegistry::CreatorMap& loaders = _loaderRegistry.getRegistrations();
        LoaderRegistry::CreatorMap::const_iterator it;
        for ( it = loaders.begin(); it != loaders.end(); ++it )
        {
            void* createSymbol = library->findSymbol( it->first );
            if ( !createSymbol ) { continue; }

            std::auto_ptr<IPluginLoader> loader( (*it->second)() );
            std::auto_ptr<IPlugin> plugin( loader->loadPlugin( createSymbol ) );
            if ( !plugin.get() )
            {
                COCA_ERROR( "Could not load plugin from '" << filename << "' using '" << it->first << "'!" );
                continue;
            }

            PluginDataPtr data( new PluginData( *library, *plugin ) );
            library.release();
            plugin.release();
            _plugins.push_back( data );
            return data.get();
        }

        return false;
    }

    bool PluginManager::removePlugin( IPluginData* data )
    {
        PluginDataPtr ptr( data );
        return eraseItem( _plugins, ptr );
    }

    bool PluginManager::removePlugin( const std::string& filename )
    {
        PluginDataVector::iterator it;
        for ( it = _plugins.begin(); it != _plugins.end(); ++it )
        {
            if ( ( *it )->getLibrary().getFilename() == filename )
            {
                _plugins.erase( it );
                return true;
            }
        }
        return false;
    }

    void PluginManager::clearPlugins()
    {
        _plugins.clear();
    }

    IPluginData* PluginManager::findPlugin( const std::string& filename ) const
    {
        PluginDataVector::const_iterator it;
        for ( it = _plugins.begin(); it != _plugins.end(); ++it )
        {
            if ( ( *it )->getLibrary().getFilename() == filename )
            {
                return ( *it ).get();
            }
        }
        return 0;
    }

    const IPluginManager::PluginDataVector& PluginManager::getPlugins() const
    {
        return _plugins;
    }

} // namespace coca

