// Copyright (C) 2007-2008 Harm Boschloo

#include "CocaDocumentManager.h"
#include "CocaView.h"
#include "CocaViewPane.h"
#include "EditorDefs.h"
#include <coca/log/log.h>
#include <coca/utils/staticCast.h>
#include <wx/aui/aui.h>

BEGIN_EVENT_TABLE( CocaDocumentManager, wxDocManager )
    EVT_UPDATE_UI( E_ID_ADD_SYSTEM_EDITOR_VIEW, CocaDocumentManager::onUpdateAddSystemEditorView )
    EVT_MENU( E_ID_ADD_SYSTEM_EDITOR_VIEW, CocaDocumentManager::onAddSystemEditorView )
    EVT_AUI_PANE_CLOSE( CocaDocumentManager::onAuiPaneClose )
END_EVENT_TABLE()

CocaDocumentManager::CocaDocumentManager()

{
}

CocaDocumentManager::~CocaDocumentManager()
{
}

void CocaDocumentManager::onUpdateAddSystemEditorView( wxUpdateUIEvent& event )
{
    event.Enable( GetCurrentDocument() != 0 );
}

void CocaDocumentManager::onAddSystemEditorView( wxCommandEvent& event )
{
    if ( GetCurrentDocument() ) { CreateView( GetCurrentDocument() ); }
}

void CocaDocumentManager::onAuiPaneClose( wxAuiManagerEvent& event )
{
    COCA_DEBUG_INFO( "CocaDocumentManager::OnAuiPaneClose " << event.GetPane() << "; " << event.CanVeto() );

    if ( !GetCurrentDocument() ) { return; }

    wxAuiPaneInfo* info = event.GetPane();

    wxList& views = GetCurrentDocument()->GetViews();

    wxList::iterator it;
    for ( it = views.begin(); it != views.end(); ++it )
    {
        CocaView* view = coca::staticCast<CocaView*>( *it );
        if ( view->getPane() == info->window )
        {
            if ( view->Close( false ) )
            {
                delete view;
            }
            else
            {
                event.Veto();
            }
            return;
        }
    }

    event.Skip(); // no related view found, skip event

}

