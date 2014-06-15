// Copyright (C) 2007-2008 Harm Boschloo

#ifndef WX_LOG_MESSAGE_ROUTER_H_INCLUDED
#define WX_LOG_MESSAGE_ROUTER_H_INCLUDED

#include <wx/log.h>

class WXLogMessageRouter : public wxLog
{
public:
    WXLogMessageRouter();
    ~WXLogMessageRouter();

protected:
    virtual void DoLogString( const wxChar* message, time_t time );
};

#endif // WX_LOG_MESSAGE_ROUTER_H_INCLUDED


