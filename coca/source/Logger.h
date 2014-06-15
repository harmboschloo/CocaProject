// Copyright (C) 2008-2009 Harm Boschloo

#ifndef COCA_LOGGER_H_INCLUDED
#define COCA_LOGGER_H_INCLUDED

#include <coca/log/ILogger.h>
#include <vector>

namespace coca
{
    class Logger : public ILogger
    {
    public:
        typedef std::vector<ILogMessageHandler*> HandlerVector;

        Logger();
        ~Logger();

        void addHandler( ILogMessageHandler& handler );
        void removeHandler( ILogMessageHandler& handler );

        void setSentry( ILoggerSentry* sentry );

        void log( const LogMessage& message );
        void log( const std::string& description, ELogMessageType type = E_INFO_LOG_MESSAGE,
                  const std::string& source = "", unsigned int line = 0 );

    private:
        ILoggerSentry* _sentry;
        HandlerVector _handlers;
    };

} // namespace coca

#endif // COCA_LOGGER_H_INCLUDED

