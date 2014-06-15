// Copyright (C) 2007-2008 Harm Boschloo

#include "WXLogMessageRouter.h"
#include <coca/log/log.h>

WXLogMessageRouter::WXLogMessageRouter()
{
}

WXLogMessageRouter::~WXLogMessageRouter()
{
}

void WXLogMessageRouter::DoLogString( const wxChar* message, time_t time )
{
    COCA_INFO( "wxLog message: " << message << " [" << time << "]" );
}
