// Copyright (C) 2008-2009 Harm Boschloo

#include <coca/interface.h>
#include "Info.h"
#include "Logger.h"
#include "EventManager.h"
#include "PluginManager.h"
#include "PluginLoader.h"
#include "Factory.h"
#include "Clock.h"
#include "Utils.h"
#include <memory>

namespace coca
{
    std::auto_ptr<Logger> _logger;
    std::auto_ptr<Utils> _utils;
    std::auto_ptr<Info> _info;
    std::auto_ptr<Clock> _clock;
    std::auto_ptr<EventManager> _eventManager;
    std::auto_ptr<PluginManager> _pluginManager;
    std::auto_ptr<Factory> _factory;

    COCA_API const IInfo& getInfo()
    {
        if ( !_info.get() ) { _info.reset( new Info() ); }
        return *_info;
    }

    COCA_API ILogger& getLogger()
    {
        if ( !_logger.get() ) { _logger.reset( new Logger() ); }
        return *_logger;
    }

    COCA_API IPluginManager& getPluginManager()
    {
        if ( !_pluginManager.get() )
        {
            _pluginManager.reset( new PluginManager() );
            _pluginManager->registerLoader( PluginLoader::CREATE_FUNCTION_NAME, &create<IPluginLoader,PluginLoader> );
        }
        return *_pluginManager;
    }

    COCA_API IFactory& getFactory()
    {
        if ( !_factory.get() ) { _factory.reset( new Factory() ); }
        return *_factory;
    }

    COCA_API IEventManager& getEventManager()
    {
        if ( !_eventManager.get() ) { _eventManager.reset( new EventManager() ); }
        return *_eventManager;
    }

    COCA_API IClock& getClock()
    {
        if ( !_clock.get() ) { _clock.reset( new Clock() ); }
        return *_clock;
    }

    COCA_API IUtils& getUtils()
    {
        if ( !_utils.get() ) { _utils.reset( new Utils() ); }
        return *_utils;
    }

    COCA_API void destroyAll()
    {
        _factory.reset();
        _pluginManager.reset();
        _eventManager.reset();
        _clock.reset();
        _info.reset();
        _utils.reset();
        _logger.reset();
    }

} // namespace coca
