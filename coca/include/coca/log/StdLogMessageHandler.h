// Copyright (C) 2008-2009 Harm Boschloo

#ifndef COCA_STD_MESSAGE_HANDLER_H_INCLUDED
#define COCA_STD_MESSAGE_HANDLER_H_INCLUDED

#include "ILogMessageHandler.h"
#include "ILogger.h"
#include "LogMessage.h"
#include "../utils/file.h"
#include <iostream>

namespace coca
{
    class StdLogMessageHandler : public ILogMessageHandler
    {
    public:
        StdLogMessageHandler()
        {
            getLogger().addHandler( *this );
        }
        virtual ~StdLogMessageHandler()
        {
            getLogger().removeHandler( *this );
        }

        virtual void onMessage( const LogMessage& message )
        {
            processMessage( message );
        }

        static void processMessage( const LogMessage& message )
        {
            switch ( message.type )
            {
            case E_ERROR_LOG_MESSAGE:
            case E_DEBUG_ERROR_LOG_MESSAGE:
                std::cerr << message << std::endl;
                break;
            default:
                std::cout << message << std::endl;
                break;
            }
        }
    };

} // namespace coca

#endif // COCA_STD_MESSAGE_HANDLER_H_INCLUDED
