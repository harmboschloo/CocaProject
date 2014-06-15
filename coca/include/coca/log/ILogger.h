// Copyright (C) 2008-2009 Harm Boschloo

#ifndef COCA_I_LOGGER_H_INCLUDED
#define COCA_I_LOGGER_H_INCLUDED

#include "../interface.h"
#include "ELogMessageType.h"
#include "../utils/NonCopyable.h"
#include <string>

namespace coca
{
    class ILogMessageHandler;
    class ILoggerSentry;
    class LogMessage;

    class ILogger : private NonCopyable
    {
    public:
        virtual void addHandler( ILogMessageHandler& ) = 0;
        virtual void removeHandler( ILogMessageHandler& ) = 0;

        virtual void setSentry( ILoggerSentry* sentry ) = 0;

        virtual void log( const LogMessage& message ) = 0;
        virtual void log( const std::string& description, ELogMessageType type = E_INFO_LOG_MESSAGE,
                          const std::string& source = "", unsigned int line = 0 ) = 0;

    protected:
        virtual ~ILogger() {};
    };

} // namespace coca

#endif // COCA_I_LOGGER_H_INCLUDED

