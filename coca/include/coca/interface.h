// Copyright (C) 2008-2009 Harm Boschloo

#ifndef COCA_INTERFACE_H_INCLUDED
#define COCA_INTERFACE_H_INCLUDED

#include "api.h"

namespace coca
{
    class IInfo;
    class ILogger;
    class IPluginManager;
    class IFactory;
    class IClock;
    class IEventManager;
    class IUtils;

    COCA_API const IInfo& getInfo();
    COCA_API ILogger& getLogger();
    COCA_API IPluginManager& getPluginManager();
    COCA_API IFactory& getFactory();
    COCA_API IEventManager& getEventManager();
    COCA_API IClock& getClock();
    COCA_API IUtils& getUtils();
    COCA_API void destroyAll();

} // namespace coca

#endif // COCA_INTERFACE_H_INCLUDED
