// Copyright (C) 2007-2008 Harm Boschloo

#include "EditorFrame.h"
#include "EditorApplication.h"
#include "EditorTools.h"
#include "EditorDefs.h"
#include "LogMessagesViewer.h"
#include <coca/log/log.h>
#include <wx/menu.h>
#include <wx/toolbar.h>
#include <wx/statusbr.h>
#include <wx/artprov.h>

BEGIN_EVENT_TABLE( EditorFrame, wxDocParentFrame )
    EVT_UPDATE_UI( E_ID_LOG_MESSAGES_VIEWER, EditorFrame::onUpdateViewWindowToggle )
    EVT_UPDATE_UI_RANGE( E_ID_FIRST_COMMAND, E_ID_LAST_COMMAND, EditorFrame::onUpdateDefault )
    EVT_MENU( E_ID_LOG_MESSAGES_VIEWER, EditorFrame::onViewWindowToggle )
END_EVENT_TABLE()

EditorFrame::EditorFrame( wxFrame* parent, int id, const wxString& title, const wxPoint& pos,
                          const wxSize& size, long style ) :
        wxDocParentFrame( &wxGetApp().getDocumentManager(), parent, id, title, pos, size, style )
{
    COCA_DEBUG_INFO( "Creating EditorFrame" );

    _auiManager.SetManagedWindow( this );
    _auiManager.SetFlags( wxAUI_MGR_DEFAULT | wxAUI_MGR_ALLOW_ACTIVE_PANE );

    SetIcon( EditorTools::getIcons().GetIcon( E_COCA_IMAGE ) );

    createMenus();
    createToolbars();
    createWindows();

    CreateStatusBar( E_NUMBER_OF_STATUS_FIELDS );

    _auiManager.Update();

    GetStatusBar()->SetStatusText( wxT( "Ready" ), E_GENERAL_STATUS_FIELD );


}

EditorFrame::~EditorFrame()
{
    COCA_DEBUG_INFO( "~EditorFrame" );
    _auiManager.UnInit();
}

void EditorFrame::createMenus()
{
    // menu bar
    wxMenuBar* menuBar = new wxMenuBar();
    SetMenuBar( menuBar );

    // file menu
    wxMenu* fileMenu = new wxMenu();
    fileMenu->Append( wxID_NEW, wxString( wxT( "New" ) ) + wxT( '\t' ) + wxT( "Ctrl-N" ),
                      wxT( "Creates a new file" ) );
    fileMenu->Append( wxID_OPEN, wxString( wxT( "Open" ) ) + wxT( '\t' ) + wxT( "Ctrl-O" ),
                      wxT( "Opens a file" ) );
    fileMenu->Append( wxID_CLOSE, wxString( wxT( "Close" ) ) + wxT( '\t' ) + wxT( "Ctrl-X" ),
                      wxT( "Closes the current file" ) );
    fileMenu->Append( wxID_SAVE, wxString( wxT( "Save" ) ) + wxT( '\t' ) + wxT( "Ctrl-S" ),
                      wxT( "Saves the current file" ) );
    fileMenu->Append( wxID_SAVEAS, wxString( wxT( "Save as" ) ) + wxT( '\t' ) + wxT( "Ctrl-Shift-S" ),
                      wxT( "Saves the current file under another filename" ) );
    fileMenu->AppendSeparator();
    fileMenu->Append( wxID_EXIT, wxString( wxT( "Quit" ) ) + wxT( '\t' ) + wxT( "Ctrl-Q" ),
                      wxT( "Quits the program" ) );

    wxGetApp().getDocumentManager().FileHistoryUseMenu( fileMenu );

    menuBar->Append( fileMenu, wxT( "File" ) );

    // view menu
    wxMenu* viewMenu = new wxMenu();
    viewMenu->AppendCheckItem( E_ID_LOG_MESSAGES_VIEWER, wxString( wxT( "log messages" ) ),
                               wxT( "Shows the log messages" ) );
    menuBar->Append( viewMenu, wxT( "View" ) );
}


void EditorFrame::createToolbars()
{
    // default toolbar
    wxAuiToolBar* toolbar = new wxAuiToolBar( this );
    toolbar->AddTool( wxID_NEW, wxT( "New" ), wxArtProvider::GetBitmap( wxART_NEW ),
                      wxT( "Creates a new file" ) );
    toolbar->AddTool( wxID_OPEN, wxT( "Open" ), wxArtProvider::GetBitmap( wxART_FILE_OPEN ),
                      wxT( "Opens a file" ) );
    toolbar->AddTool( wxID_SAVE, wxT( "Save" ), wxArtProvider::GetBitmap( wxART_FILE_SAVE ),
                      wxT( "Saves the current file" ) );
    toolbar->AddTool( wxID_SAVEAS, wxT( "Save As" ), wxArtProvider::GetBitmap( wxART_FILE_SAVE_AS ),
                      wxT( "Saves the current file under another filename" ) );
    toolbar->Realize();

    _auiManager.AddPane(
        toolbar, wxAuiPaneInfo().
        Name( wxT( "toolbar" ) ).
        Caption( wxT( "Toolbar" ) ).
        ToolbarPane().Top().
        LeftDockable( false ).
        RightDockable( false )
    );

    // document toolbar
    toolbar = new wxAuiToolBar( this );
    toolbar->AddTool( E_ID_MOVE_NODE_UP, wxT( "Move node up" ), wxArtProvider::GetBitmap( wxART_GO_UP ),
                      wxT( "Moves the current node up" ) );
    toolbar->AddTool( E_ID_MOVE_NODE_DOWN, wxT( "Move node down" ), wxArtProvider::GetBitmap( wxART_GO_DOWN ),
                      wxT( "Moves the current node down" ) );
    toolbar->AddTool( E_ID_REMOVE_NODE, wxT( "Remove node" ), wxArtProvider::GetBitmap( wxART_DELETE ),
                      wxT( "Removes the current node from the system" ) );
    toolbar->AddSeparator();
    toolbar->AddTool( E_ID_ADD_SYSTEM_EDITOR_VIEW, wxT( "Add system editor" ), wxArtProvider::GetBitmap( wxART_REPORT_VIEW ),
                      wxT( "Adds another system editor" ) );
    toolbar->Realize();

    _auiManager.AddPane(
        toolbar, wxAuiPaneInfo().
        Name( wxT( "document_toolbar" ) ).
        Caption( wxT( "Document toolbar" ) ).
        ToolbarPane().Top().
        LeftDockable( false ).
        RightDockable( false )
    );

}

void EditorFrame::createWindows()
{
    // messages window
    LogMessagesViewer* messagesViewer = new LogMessagesViewer( this, E_ID_LOG_MESSAGES_VIEWER );
    _auiManager.AddPane(
        messagesViewer, wxAuiPaneInfo().
        Name( wxT( "log_messages_viewer" ) ).
        Caption( wxT( "log messages" ) ).
        Bottom().
        Row( 0 ).
        BestSize( 600, 300 ).
        CloseButton( true ).
        MaximizeButton( true )
    );
}

void EditorFrame::onUpdateViewWindowToggle( wxUpdateUIEvent& event )
{
    wxWindow* window = FindWindow( event.GetId() );
    event.Enable( window != 0 );
    event.Check( window != 0 && _auiManager.GetPane( window ).IsShown() );
}

void EditorFrame::onUpdateDefault( wxUpdateUIEvent& event )
{
    event.Enable( false );
}

void EditorFrame::onViewWindowToggle( wxCommandEvent& event )
{
    wxWindow* window = FindWindow( event.GetId() );
    if ( window )
    {
        wxAuiPaneInfo& pane = _auiManager.GetPane( window );
        pane.Show( !pane.IsShown() );
        _auiManager.Update();
    }
}

wxAuiManager& EditorFrame::getAuiManager()
{
    return _auiManager;
}
