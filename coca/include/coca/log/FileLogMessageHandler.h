// Copyright (C) 2008-2009 Harm Boschloo

#ifndef COCA_FILE_LOG_MESSAGE_HANDLER_H_INCLUDED
#define COCA_FILE_LOG_MESSAGE_HANDLER_H_INCLUDED

#include "ILogMessageHandler.h"
#include "ILogger.h"
#include "LogMessage.h"
#include "../utils/file.h"
#include <string>
#include <fstream>
#include <ctime>

namespace coca
{
    class FileLogMessageHandler : public ILogMessageHandler
    {
    public:
        FileLogMessageHandler( std::string filename ) :
                _file( filename.c_str() )
        {
#ifdef _DEBUG
            _file << "File opened (debug build), ";
#else
            _file << "File opened (release build), ";
#endif // _DEBUG

            _file << clock() << ", " << getCurrentTimeString();
            getLogger().addHandler( *this );
        }

        virtual ~FileLogMessageHandler()
        {
            getLogger().removeHandler( *this );
            _file << "File closed, " << clock() << ", " << getCurrentTimeString();
        }

        std::ofstream& getStream()
        {
            return _file;
        }

        virtual void onMessage( const LogMessage& message )
        {
            _file << message << std::endl;
        }

    private:
		std::string getCurrentTimeString()
		{
			time_t rawtime = time( 0 );
#ifdef _MSC_VER
			tm time;
			char buffer[32];
			buffer[0] = '\0';
			localtime_s( &time, &rawtime );
			asctime_s( buffer, 32, &time );
			return std::string( buffer );
#else
			return asctime( localtime( &rawtime ) );
#endif
		}

        std::ofstream _file;
    };

} // namespace coca

#endif // COCA_FILE_LOG_MESSAGE_HANDLER_H_INCLUDED
