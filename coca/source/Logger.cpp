// Copyright (C) 2008-2009 Harm Boschloo

#include "Logger.h"
#include "ScopedLogSentry.h"
#include <coca/log/ILogMessageHandler.h>
#include <coca/log/LogMessage.h>
#include <coca/log/StdLogMessageHandler.h>
#include <coca/utils/algorithm.h>
#include <coca/utils/function/autogen/function.h>

namespace coca
{
    Logger::Logger() :
            _sentry( 0 )
    {
    }

    Logger::~Logger()
    {
    }

    void Logger::addHandler( ILogMessageHandler& handler )
    {
        ScopedLogSentry sentry( _sentry );
        _handlers.push_back( &handler );
    }

    void Logger::removeHandler( ILogMessageHandler& handler )
    {
        ScopedLogSentry sentry( _sentry );
        eraseItem( _handlers, &handler );
    }

    void Logger::setSentry( ILoggerSentry* sentry )
    {
        _sentry = sentry;
    }

    void Logger::log( const LogMessage& message )
    {
        ScopedLogSentry sentry( _sentry );

        if ( _handlers.empty() )
        {
            StdLogMessageHandler::processMessage( message );
        }
        else
        {
            forAll( _handlers, makePtrMemberFunction( &ILogMessageHandler::onMessage ), message );
        }
    }

    void Logger::log( const std::string& description, ELogMessageType type,
                      const std::string& source, unsigned int line )
    {
        LogMessage message( description, type, source, line );
        log( message );
    }

} // namespace coca
