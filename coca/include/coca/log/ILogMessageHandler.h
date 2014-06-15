// Copyright (C) 2008-2009 Harm Boschloo

#ifndef COCA_I_LOG_MESSAGE_HANDLER_H_INCLUDED
#define COCA_I_LOG_MESSAGE_HANDLER_H_INCLUDED

namespace coca
{
    class LogMessage;

    class ILogMessageHandler
    {
    public:
        virtual ~ILogMessageHandler() {};
        virtual void onMessage( const LogMessage& message ) = 0;
    };

} // namespace coca

#endif // COCA_I_LOG_MESSAGE_HANDLER_H_INCLUDED

