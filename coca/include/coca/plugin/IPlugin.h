// Copyright (C) 2008-2009 Harm Boschloo

#ifndef COCA_I_PLUGIN_H_INCLUDED
#define COCA_I_PLUGIN_H_INCLUDED

#include "../utils/NonCopyable.h"

namespace coca
{
    class IPluginInfo;

    class IPlugin : private NonCopyable
    {
    public:
        virtual ~IPlugin() {}
        virtual const IPluginInfo& getInfo() const = 0;
        virtual bool init() = 0;
    };

} // namespace coca

#endif // COCA_I_PLUGIN_H_INCLUDED
