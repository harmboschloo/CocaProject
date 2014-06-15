// Copyright (C) 2008-2009 Harm Boschloo

#ifndef COCA_PLUGIN_LOADER_H_INCLUDED
#define COCA_PLUGIN_LOADER_H_INCLUDED

#include <coca/plugin/IPluginLoader.h>
#include <string>

namespace coca
{
    class APlugin;

    class PluginLoader : public IPluginLoader
    {
    public:
        typedef APlugin* (*CreateFunctionPointer)();

        static const std::string CREATE_FUNCTION_NAME;

        PluginLoader();
        ~PluginLoader();

        IPlugin* loadPlugin( void* createSymbol );

    };

} // namespace coca

#endif //COCA_PLUGIN_LOADER_H_INCLUDED
