// Copyright (C) 2008-2009 Harm Boschloo

#ifndef COCA_LOG_LOG_H_INCLUDED
#define COCA_LOG_LOG_H_INCLUDED

#include "ILogger.h"
#include "LogMessage.h"
#include <sstream>

#define COCA_INFO( description ) { std::ostringstream  ss; ss << description; \
        coca::getLogger().log( COCA_INFO_LOG_MESSAGE( ss.str() ) ); }
#define COCA_WARNING( description ) { std::ostringstream  ss; ss << description; \
        coca::getLogger().log( COCA_WARNING_LOG_MESSAGE( ss.str() ) ); }
#define COCA_ERROR( description ) { std::ostringstream  ss; ss << description; \
        coca::getLogger().log( COCA_ERROR_LOG_MESSAGE( ss.str() ) ); }

#ifdef NDEBUG
#define COCA_DEBUG_INFO( description ) {}
#define COCA_DEBUG_WARNING( description ) {}
#define COCA_DEBUG_ERROR( description ) {}
#else
#define COCA_DEBUG_INFO( description ) { std::ostringstream  ss; ss << description; \
        coca::getLogger().log( COCA_DEBUG_INFO_LOG_MESSAGE( ss.str() ) ); }
#define COCA_DEBUG_WARNING( description ) { std::ostringstream  ss; ss << description; \
        coca::getLogger().log( COCA_DEBUG_WARNING_LOG_MESSAGE( ss.str() ) ); }
#define COCA_DEBUG_ERROR( description ) { std::ostringstream  ss; ss << description; \
        coca::getLogger().log( COCA_DEBUG_ERROR_LOG_MESSAGE( ss.str() ) ); }
#endif // NDEBUG

#endif // COCA_LOG_LOG_H_INCLUDED

