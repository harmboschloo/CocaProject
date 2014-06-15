// Copyright (C) 2008-2009 Harm Boschloo

#ifndef COCA_I_PLUGIN_LOADER_H_INCLUDED
#define COCA_I_PLUGIN_LOADER_H_INCLUDED

#include "../utils/NonCopyable.h"

namespace coca
{
    class IPlugin;

    class IPluginLoader : private NonCopyable
    {
    public:
        virtual ~IPluginLoader() {}
        virtual IPlugin* loadPlugin( void* createSymbol ) = 0;
    };

} // namespace coca

#endif // COCA_I_PLUGIN_LOADER_H_INCLUDED
