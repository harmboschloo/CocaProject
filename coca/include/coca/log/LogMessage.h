// Copyright (C) 2008-2009 Harm Boschloo

#ifndef COCA_LOG_MESSAGE_H_INCLUDED
#define COCA_LOG_MESSAGE_H_INCLUDED

#include "ELogMessageType.h"
#include "../utils/file.h"
#include <string>
#include <ctime>
#include <ostream>

#define COCA_LOG_HERE __FILE__, __LINE__
#define COCA_INFO_LOG_MESSAGE( description ) description, coca::E_INFO_LOG_MESSAGE, COCA_LOG_HERE
#define COCA_WARNING_LOG_MESSAGE( description ) description, coca::E_WARNING_LOG_MESSAGE, COCA_LOG_HERE
#define COCA_ERROR_LOG_MESSAGE( description ) description, coca::E_ERROR_LOG_MESSAGE, COCA_LOG_HERE
#define COCA_DEBUG_INFO_LOG_MESSAGE( description ) description, coca::E_DEBUG_INFO_LOG_MESSAGE, COCA_LOG_HERE
#define COCA_DEBUG_WARNING_LOG_MESSAGE( description ) description, coca::E_DEBUG_WARNING_LOG_MESSAGE, COCA_LOG_HERE
#define COCA_DEBUG_ERROR_LOG_MESSAGE( description ) description, coca::E_DEBUG_ERROR_LOG_MESSAGE, COCA_LOG_HERE

namespace coca
{
    class LogMessage
    {
    public:
        LogMessage() :
                type( E_INFO_LOG_MESSAGE ),
                line( 0 ),
                time( 0 ),
                ticks( clock() )
        {
        }

        LogMessage( const std::string& description_, ELogMessageType type_ = E_INFO_LOG_MESSAGE,
                    const std::string& source_ = "", unsigned int line_ = 0 ) :
                description( description_ ),
                type( type_ ),
                source( source_ ),
                line( line_ ),
                time( 0 ),
                ticks( clock() )
        {
        }

        LogMessage( const LogMessage& other ) :
                description( other.description ),
                type( other.type ),
                source( other.source ),
                line( other.line ),
                time( other.time ),
                ticks( other.ticks )
        {
        }

        LogMessage& operator=( const LogMessage& other )
        {
            description = other.description;
            type = other.type;
            source = other.source;
            line = other.line;
            time = other.time;
            ticks = other.ticks;
            return *this;
        }

    public:
        std::string description;
        ELogMessageType type;
        std::string source;
        unsigned int line;
        time_t time;
        clock_t ticks;
    };

} // namespace coca

inline std::ostream& operator<<( std::ostream& stream, const coca::LogMessage& message )
{
    switch ( message.type )
    {
    case coca::E_INFO_LOG_MESSAGE:
        stream << "I: ";
        break;
    case coca::E_WARNING_LOG_MESSAGE:
        stream << "W: ";
        break;
    case coca::E_ERROR_LOG_MESSAGE:
        stream << "E: ";
        break;
    case coca::E_DEBUG_INFO_LOG_MESSAGE:
        stream << "DI: ";
        break;
    case coca::E_DEBUG_WARNING_LOG_MESSAGE:
        stream << "DW: ";
        break;
    case coca::E_DEBUG_ERROR_LOG_MESSAGE:
        stream << "DE: ";
        break;
    default:
        stream << "UNKOWN: ";
        break;
    }

    return stream << message.description << " [" << message.ticks << ", "
    << coca::extractFilename( message.source ) << ", line " << message.line << "]";
}

#endif // COCA_LOG_MESSAGE_H_INCLUDED

