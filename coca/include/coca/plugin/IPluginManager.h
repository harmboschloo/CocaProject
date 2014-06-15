// Copyright (C) 2008-2009 Harm Boschloo

#ifndef COCA_I_PLUGIN_MANAGER_H_INCLUDED
#define COCA_I_PLUGIN_MANAGER_H_INCLUDED

#include "../interface.h"
#include "PluginDataPtr.h"
#include "../utils/NonCopyable.h"
#include "../utils/FactoryRegistry.h"
#include "../utils/create.h"
#include <string>
#include <vector>

namespace coca
{
    class IPluginLoader;

    class IPluginManager : private NonCopyable
    {
    public:
        /** @name Plugin Loaders */
        //@{
        typedef FactoryRegistry<IPluginLoader> LoaderRegistry;

        template<typename T>
        inline bool registerLoader( const std::string& createFunctionName )
        {
            return registerLoader( createFunctionName, &create<IPluginLoader,T> );
        }

        virtual bool registerLoader( const std::string& createFunctionName,
                                     LoaderRegistry::Creator creator ) = 0;
        virtual const LoaderRegistry& getLoaderRegistry() const = 0;
        //@}

        /** @name Plugins */
        //@{
        typedef std::vector<PluginDataPtr> PluginDataVector;

        /// the default plugin location: [coca library path]/plugins
        virtual std::string getDefaultPluginDirectory() = 0;
        /// loads the plugins from the default location
        virtual int loadPlugins() = 0;
        virtual int loadPlugins( const std::string& directory ) = 0;
        virtual IPluginData* loadPlugin( const std::string& filename ) = 0;
        virtual bool removePlugin( IPluginData* data ) = 0;
        virtual bool removePlugin( const std::string& filename ) = 0;
        virtual void clearPlugins() = 0;
        virtual IPluginData* findPlugin( const std::string& filename ) const = 0;
        virtual const PluginDataVector& getPlugins() const = 0;
        //@}

    protected:
        virtual ~IPluginManager() {}
    };

} // namespace coca

#endif //COCA_I_PLUGIN_MANAGER_H_INCLUDED
