// Copyright (C) 2008-2009 Harm Boschloo

#ifndef COCA_I_PLUGIN_DATA_H_INCLUDED
#define COCA_I_PLUGIN_DATA_H_INCLUDED

#include "../utils/ReferenceCounted.h"

namespace coca
{
    class ILibrary;
    class IPlugin;

    class IPluginData : public ReferenceCounted
    {
    public:
        virtual const ILibrary& getLibrary() const = 0;
        virtual const IPlugin& getPlugin() const = 0;

    protected:
        virtual ~IPluginData() {}
    };

} // namespace coca

#endif // COCA_I_PLUGIN_DATA_H_INCLUDED

