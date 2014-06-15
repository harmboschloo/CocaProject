// Copyright (C) 2007-2008 Harm Boschloo

#include "CocaView.h"
#include "CocaViewPane.h"
#include "CocaDocument.h"
#include "CocaDocumentEvent.h"
#include <coca/log/log.h>
#include <coca/utils/test.h>
#include <coca/utils/staticCast.h>

IMPLEMENT_DYNAMIC_CLASS( CocaView, wxView )

BEGIN_EVENT_TABLE( CocaView, wxView )
END_EVENT_TABLE()

CocaView::CocaView()
{
    COCA_DEBUG_INFO( "CocaView Created " << this );
}

CocaView::~CocaView()
{
    COCA_DEBUG_INFO( "CocaView Destroyed " << this );
}

void CocaView::Activate( bool activate )
{
    COCA_DEBUG_INFO( "CocaView::Activate " << this << "; " << activate );
    COCA_ASSERT( GetDocumentManager() );

    CocaView* oldView = coca::staticCast<CocaView*>( GetDocumentManager()->GetCurrentView() );
    wxView::Activate( activate );
    CocaView* currentView = coca::staticCast<CocaView*>( GetDocumentManager()->GetCurrentView() );

    if ( oldView ) { oldView->onDeactivated(); }
    if ( currentView ) { currentView->onActivated(); }
}

void CocaView::OnUpdate( wxView *sender, wxObject *hint )
{
    COCA_DEBUG_INFO( "CocaView::OnUpdate " << this << "; " << sender << "; " << hint );
    if ( !hint ) { return; }

    CocaDocumentEvent* event = coca::staticCast<CocaDocumentEvent*>( hint );
    if ( !processEvent( *event ) ) { CocaView::processEvent( *event ); }
}

bool CocaView::processEvent( CocaDocumentEvent& event )
{
    COCA_DEBUG_INFO( "CocaView::processEvent " << this << "; " << event.getType() );

    switch ( event.getType() )
    {
    default:
        break;
    }

    return true;
}

CocaViewPane* CocaView::getPane()
{
    return 0;
}

void CocaView::readConfig( wxConfigBase&, wxString )
{
}

void CocaView::writeConfig( wxConfigBase&, wxString )
{
}

CocaDocument& CocaView::getDocument() const
{
    COCA_ASSERT( GetDocument() );
    return *coca::staticCast<CocaDocument*>( GetDocument() );
}
bool CocaView::isActive() const
{
    return ( GetDocumentManager()->GetCurrentView() == this );
}

// required //

void CocaView::OnDraw( wxDC* )
{
    //COCA_DEBUG_INFO( "CocaView::OnDraw" );
}

// debug //

void CocaView::OnActivateView( bool activate, wxView* activeView, wxView* deactiveView )
{
    wxString title;
    GetDocument()->GetPrintableName( title );
    COCA_DEBUG_INFO( "CocaView::OnActivateView " << this << "; " << title << " " << activate << " " << activeView
                     << " " << deactiveView );
}

void CocaView::OnChangeFilename()
{
    COCA_DEBUG_INFO( "CocaView::OnChangeFilename " << this << "; " << GetDocument()->GetFilename() );
    wxView::OnChangeFilename();
}

void CocaView::OnPrint( wxDC *dc, wxObject *info )
{
    COCA_DEBUG_INFO( "CocaView::OnPrint " << this << "; " );
    wxView::OnPrint( dc, info );
}

bool CocaView::OnCreate( wxDocument *doc, long flags )
{
    COCA_DEBUG_INFO( "CocaView::OnCreate " << this << "; " );
    return wxView::OnCreate( doc,flags );
}

bool CocaView::Close( bool deleteWindow )
{
    COCA_DEBUG_INFO( "CocaView::Close " << this );
    return wxView::Close( deleteWindow );
}

bool CocaView::OnClose( bool deleteWindow )
{
    COCA_DEBUG_INFO( "CocaView::OnClose " << this << "; " );
    return wxView::OnClose( deleteWindow );
}

void CocaView::OnClosingDocument()
{
    COCA_DEBUG_INFO( "CocaView::OnClosingDocument " << this );
    wxView::OnClosingDocument();
}

