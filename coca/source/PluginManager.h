// Copyright (C) 2008-2009 Harm Boschloo

#ifndef COCA_PLUGIN_MANAGER_H_INCLUDED
#define COCA_PLUGIN_MANAGER_H_INCLUDED

#include <coca/plugin/IPluginManager.h>
#include <string>
#include <vector>

namespace coca
{
    class IPluginLoader;

    class PluginManager : public IPluginManager
    {
    public:
        PluginManager();
        ~PluginManager();

        bool registerLoader( const std::string& createFunctionName, LoaderRegistry::Creator creator );
        const LoaderRegistry& getLoaderRegistry() const;

        std::string getDefaultPluginDirectory();
        int loadPlugins();
        int loadPlugins( const std::string& directory );
        IPluginData* loadPlugin( const std::string& filename );
        bool removePlugin( IPluginData* data );
        bool removePlugin( const std::string& filename );
        void clearPlugins();
        IPluginData* findPlugin( const std::string& filename ) const;
        const PluginDataVector& getPlugins() const;

    private:
        LoaderRegistry _loaderRegistry;
        PluginDataVector _plugins;
    };

} // namespace coca

#endif //COCA_PLUGIN_MANAGER_H_INCLUDED
