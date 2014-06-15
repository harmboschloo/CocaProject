// Copyright (C) 2007-2008 Harm Boschloo

#include "CocaViewPane.h"
#include "CocaView.h"
#include "EditorApplication.h"
#include "EditorFrame.h"
#include <coca/log/log.h>

IMPLEMENT_CLASS( CocaViewPane, wxPanel )

BEGIN_EVENT_TABLE( CocaViewPane, wxPanel )
    EVT_IDLE( CocaViewPane::onIdle )
    EVT_CHILD_FOCUS( CocaViewPane::onChildFocus )
END_EVENT_TABLE()

CocaViewPane::CocaViewPane( CocaView& view,
                            wxWindow* parent,
                            wxWindowID id,
                            const wxPoint& pos,
                            const wxSize& size,
                            long style,
                            const wxString& name )
        : wxPanel( parent, id, pos, size, style, name ),
        _view( &view )
{
    COCA_DEBUG_INFO( "CocaViewPane::CocaViewPane " << this );
}

CocaViewPane::~CocaViewPane()
{
    COCA_DEBUG_INFO( "CocaViewPane::~CocaViewPane " << this );
}

// Extend event processing to search the view's event table
bool CocaViewPane::ProcessEvent( wxEvent& event )
{
    if ( _view->ProcessEvent( event ) ) { return true; }
    else { return wxEvtHandler::ProcessEvent( event ); }
}

void CocaViewPane::onIdle( wxIdleEvent& event )
{
    // this seems to be the only reliable way to make sure the current view pane is active

    wxAuiPaneInfo& info = wxGetApp().getFrame()->getAuiManager().GetPane( this );
    if ( !info.IsOk() ) { return; }

    if ( ( info.state&wxAuiPaneInfo::optionActive ) && !_view->isActive() )
    {
        _view->Activate( true );
    }

    event.Skip();
}

void CocaViewPane::onChildFocus( wxChildFocusEvent& event )
{
    //COCA_DEBUG_INFO( "CocaViewPane::onChildFocus " << this << "; " << &event );

    if ( !_view->isActive() ) { _view->Activate( true ); }
    event.Skip();
}

CocaDocument& CocaViewPane::getDocument() const
{
    return _view->getDocument();
}

CocaView& CocaViewPane::getView() const
{
    return *_view;
}

