// Copyright (C) 2008-2009 Harm Boschloo

#ifndef COCA_PLUGIN_DATA_H_INCLUDED
#define COCA_PLUGIN_DATA_H_INCLUDED

#include <coca/plugin/IPluginData.h>
#include <coca/plugin/ILibrary.h>
#include <coca/plugin/IPlugin.h>

namespace coca
{
    class PluginData : public IPluginData
    {
    public:
        PluginData( ILibrary& library, IPlugin& plugin ) :
                _library( &library ),
                _plugin( &plugin )
        {
        }

        ~PluginData()
        {
            delete _plugin;
            delete _library;
        }

        const ILibrary& getLibrary() const
        {
            return *_library;
        }

        const IPlugin& getPlugin() const
        {
            return *_plugin;
        }

    private:
        ILibrary* _library;
        IPlugin* _plugin;
    };

} // namespace coca

#endif // COCA_PLUGIN_DATA_H_INCLUDED

