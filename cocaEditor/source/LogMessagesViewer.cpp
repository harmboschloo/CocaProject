// Copyright (C) 2007-2008 Harm Boschloo

#include "LogMessagesViewer.h"
#include <coca/log/log.h>
#include <wx/thread.h>
#include <wx/timer.h>
#include <sstream>

BEGIN_EVENT_TABLE( LogMessagesViewer, wxGrid )
    EVT_TIMER( wxID_ANY, LogMessagesViewer::onTimer )
END_EVENT_TABLE()

LogMessagesViewer::LogMessagesViewer( wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxSize& size, long style, const wxString& name )
        : wxGrid( parent, id, pos, size, style, name ),
        _timer( this )
{
    // grid
    CreateGrid( 0, 4 );
    EnableEditing( false );
    EnableGridLines( false );
    EnableDragGridSize( false );
    SetMargins( 0, 0 );
    // columns
    //EnableDragColMove( false );
    EnableDragColSize( true );
    SetColLabelSize( 20 );
    SetColLabelAlignment( wxALIGN_CENTRE, wxALIGN_CENTRE );
    // rows
    EnableDragRowSize( true );
    SetRowLabelSize( 40 );
    SetRowLabelAlignment( wxALIGN_CENTRE, wxALIGN_CENTRE );
    // label appearance
    SetColLabelValue( 0, _T( "description" ) );
    SetColLabelValue( 1, _T( "source" ) );
    SetColLabelValue( 2, _T( "line" ) );
    SetColLabelValue( 3, _T( "clock" ) );
    // cell defaults
    SetDefaultCellAlignment( wxALIGN_LEFT, wxALIGN_TOP );

    coca::getLogger().addHandler( *this );

    _timer.Start( 200 );
}

LogMessagesViewer::~LogMessagesViewer()
{
    coca::getLogger().removeHandler( *this );
}

void LogMessagesViewer::onTimer( wxTimerEvent& event )
{
    appendMessages();
}

void LogMessagesViewer::onMessage( const coca::LogMessage& message )
{
    wxCriticalSectionLocker lock( _messagesCS );

    _messages.push( message );
    std::ostringstream ss;
    ss << "[thread " << wxThread::GetCurrentId() << "] ";
    _messages.back().description.insert( 0, ss.str() );
}

void LogMessagesViewer::appendMessages()
{
    wxCriticalSectionLocker lock( _messagesCS );

    if ( _messages.empty() ) { return; }

    BeginBatch();

    int row = GetNumberRows();
    AppendRows( _messages.size() );

    do
    {
        appendMessage( _messages.front(), row );
        _messages.pop();
        ++row;
    }
    while ( !_messages.empty() );

    AutoSizeColumns();

    EndBatch();

    MakeCellVisible( GetNumberRows()-1, 0 );
}

void LogMessagesViewer::appendMessage( const coca::LogMessage& message, int row )
{
    SetCellValue( row, 0, _T( message.description ) );
    SetCellValue( row, 1, _T( coca::extractFilename( message.source ) ) );
    std::ostringstream ss;
    ss << message.line;
    SetCellValue( row, 2, _T( ss.str() ) );
    ss.str( "" );
    ss << message.ticks;
    SetCellValue( row, 3, _T( ss.str() ) );

    if ( message.type == coca::E_WARNING_LOG_MESSAGE )
    {
        wxGridCellAttr* attr = GetOrCreateCellAttr( row, 0 );
        attr->SetTextColour( wxColour( 0, 0, 255 ) );
        SetRowAttr( row, attr );
    }
    else if ( message.type == coca::E_ERROR_LOG_MESSAGE )
    {
        wxGridCellAttr* attr = GetOrCreateCellAttr( row, 0 );
        attr->SetTextColour( wxColour( 255, 0, 0 ) );
        SetRowAttr( row, attr );
    }

}
