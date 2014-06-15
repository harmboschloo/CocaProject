// Copyright (C) 2007-2008 Harm Boschloo

#ifndef LOG_MESSAGES_VIEWER_H_INCLUDED
#define LOG_MESSAGES_VIEWER_H_INCLUDED

#include <coca/log/ILogMessageHandler.h>
#include <coca/log/LogMessage.h>
#include <wx/grid.h>
#include <wx/timer.h>
#include <queue>

class wxTimerEvent;

class LogMessagesViewer : public wxGrid, public coca::ILogMessageHandler
{
public:
    LogMessagesViewer(
        wxWindow* parent,
        wxWindowID id = wxID_ANY,
        const wxPoint& pos = wxDefaultPosition,
        const wxSize& size = wxDefaultSize,
        long style = wxWANTS_CHARS,
        const wxString& name = wxPanelNameStr
    );
    virtual ~LogMessagesViewer();

    void onTimer( wxTimerEvent& event );
    void onMessage( const coca::LogMessage& message );

private:
    void appendMessages();
    void appendMessage( const coca::LogMessage& message, int row );

    wxCriticalSection _messagesCS;
    std::queue<coca::LogMessage> _messages;
    wxTimer _timer;

    DECLARE_EVENT_TABLE()
};

#endif // LOG_MESSAGES_VIEWER_H_INCLUDED
