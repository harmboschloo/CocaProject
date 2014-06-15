// Copyright (C) 2007-2008 Harm Boschloo

#include "CocaDocument.h"
#include "CocaDocumentEvent.h"
#include "CocaView.h"
#include "CocaViewPane.h"
#include "CocaResourceManager.h"
#include "coca_commands.h"
#include "EditorApplication.h"
#include "EditorFrame.h"
#include "EditorDefs.h"
#include "EditorTools.h"
#include "ScopedWXBusyCursor.h"
#include <coca/IFactory.h>
#include <coca/IClock.h>
#include <coca/log/log.h>
#include <coca/utils/staticCast.h>
#include <coca/utils/string.h>
#include <wx/filename.h>
#include <wx/statusbr.h>
#include <wx/timer.h>
#include <wx/fileconf.h>
#include <wx/filefn.h>
#include <wx/settings.h>

bool CocaDocument::EventComparator::operator()( const EventPtr& event1, const EventPtr& event2 ) const
{
    return getPriority( event1 ) > getPriority( event2 );
}

int CocaDocument::EventComparator::getPriority( const EventPtr& event ) const
{
    switch ( event->getType() )
    {
    case E_SYSTEM_CHANGED_EVENT:
    case coca::E_SYSTEM_ROOT_CHANGED_EVENT:
        return 1;
    case coca::E_NODE_CHILD_REMOVED_EVENT:
        return 2;
    case coca::E_NODE_COMPONENT_CHANGED_EVENT:
        return 3;
    case coca::E_NODE_ATTRIBUTE_REMOVED_EVENT:
        return 4;
    case coca::E_ATTRIBUTE_SINK_REMOVED_EVENT:
    case coca::E_ATTRIBUTE_SOURCE_REMOVED_EVENT:
        return 5;
    case coca::E_SYSTEM_DESCRIPTION_CHANGED_EVENT:
    case coca::E_NODE_NAME_CHANGED_EVENT:
    case coca::E_NODE_DISABLED_COUNT_CHANGED_EVENT:
    case coca::E_NODE_CHILD_APPENDED_EVENT:
    case coca::E_NODE_CHILD_MOVED_UP_EVENT:
    case coca::E_NODE_CHILD_MOVED_DOWN_EVENT:
    case coca::E_NODE_ATTRIBUTE_ADDED_EVENT:
    case coca::E_ATTRIBUTE_ENABLED_EVENT:
    case coca::E_ATTRIBUTE_DISABLED_EVENT:
    case coca::E_ATTRIBUTE_SINK_APPENDED_EVENT:
    case coca::E_ATTRIBUTE_SINK_MOVED_UP_EVENT:
    case coca::E_ATTRIBUTE_SINK_MOVED_DOWN_EVENT:
    case coca::E_ATTRIBUTE_SOURCE_APPENDED_EVENT:
    case E_DOCUMENT_MODIFIED_CHANGED_EVENT:
    case E_ATTRIBUTE_VALUE_CHANGED_EVENT:
        return 98;
    default:
        COCA_ERROR( "Unhandled event!" );
        return 99;
    }
}

const int CocaDocument::CONFIG_LAYOUT_VERSION = 2;

IMPLEMENT_DYNAMIC_CLASS( CocaDocument, wxDocument )

BEGIN_EVENT_TABLE( CocaDocument, wxDocument )
    EVT_IDLE( CocaDocument::onIdle )
    EVT_TIMER( wxID_ANY, CocaDocument::onTimer )
    EVT_MENU( E_ID_EDIT_SYSTEM_DESCRIPTION, CocaDocument::onEditSystemDescription )
    EVT_MENU( E_ID_EDIT_SYSTEM_ROOT, CocaDocument::onEditSystemRoot )
END_EVENT_TABLE()

CocaDocument::CocaDocument() :
        _system( 0 ),
        _eventFunction( coca::makeFunction( *this, &CocaDocument::onCocaEvent ) ),
        _timer( this )
{
    COCA_DEBUG_INFO( "CocaDocument::CocaDocument " << this );

    resetClock();
    _timer.Start( 1 );

    COCA_DEBUG_INFO( "end CocaDocument::CocaDocument " << this );
}

CocaDocument::~CocaDocument()
{
    COCA_DEBUG_INFO( "CocaDocument::~CocaDocument " << this );

    EditorTools::updateFrameTitle();
    destroySystem();
    resetClock();

    COCA_DEBUG_INFO( "end CocaDocument::~CocaDocument " << this );
}

// wxDocument callbacks //

bool CocaDocument::OnNewDocument()
{
    COCA_DEBUG_INFO( "CocaDocument::OnNewDocument" );

    COCA_ASSERT_EQUAL( _system, ( void* )0 );
    createSystem();
    setModified( false );

    return wxDocument::OnNewDocument();
}

bool CocaDocument::DoSaveDocument( const wxString& filename )
{
    COCA_DEBUG_INFO( "CocaDocument::DoSaveDocument " << filename );

    ScopedWXBusyCursor busyCursor;
    return saveSystem( filename.c_str() );
}

bool CocaDocument::DoOpenDocument( const wxString& filename )
{
    COCA_DEBUG_INFO( "CocaDocument::DoOpenDocument " << filename );

    ScopedWXBusyCursor busyCursor;
    return loadSystem( filename.c_str() );
}

bool CocaDocument::OnCloseDocument()
{
    COCA_DEBUG_INFO( "CocaDocument::OnCloseDocument " << this << "; " << m_documentViews.GetCount() );

    writeConfig( GetFilename().c_str() );
    return wxDocument::OnCloseDocument();
}

// wx events //

void CocaDocument::onIdle( wxIdleEvent& event )
{
    updateClock();
    executeCommands();
    updateViews();
    event.RequestMore();
    event.Skip();
}

// makes sure the clock is updated even if a popup menu is displayed (onIdle won't be called then)
void CocaDocument::onTimer( wxTimerEvent& event )
{
    updateClock();
    updateViews();
    executeCommands();
}

void CocaDocument::onEditSystemDescription( wxCommandEvent& event )
{
    editSystemDescription();
}

void CocaDocument::onEditSystemRoot( wxCommandEvent& event )
{
    editSystemRoot();
}

// coca events //

void CocaDocument::onCocaEvent( const coca::IEvent& event )
{
    // this is called from the CocaThread!

    COCA_DEBUG_INFO( "CocaDocument::onCocaEvent " << event.getType() );
    _events.push( new CocaDocumentEvent( event ) );
}

// queries //

const coca::ISystem* CocaDocument::getSystem() const
{
    return _system;
}

wxString CocaDocument::getName( bool prependModified ) const
{
    wxString name;
    GetPrintableName( name );
    if ( prependModified && IsModified() )
    {
        name.Prepend( "*" );
    }
    return name;
}

// commands //

void CocaDocument::post( const CommandPtr& command )
{
    wxGetApp().getResourceManager().postCommand( command );
}

void CocaDocument::executeCommands()
{
    wxGetApp().getResourceManager().executeCommands();
}

bool CocaDocument::execute( const CommandPtr& command )
{
    return wxGetApp().getResourceManager().executeCommand( command );
}

void CocaDocument::setModified( bool modified )
{
    if ( modified != IsModified() )
    {
        Modify( modified );
        updateViews( E_DOCUMENT_MODIFIED_CHANGED_EVENT );
    }
}

void CocaDocument::updateClock()
{
    execute( new ClockTickCommand() );
    updateTickRateStatus();
}

void CocaDocument::resetClock()
{
    execute( new ResetClockCommand() );
    updateTickRateStatus();
}

void CocaDocument::createSystem()
{
    destroySystem();
    execute( new CreateSystemCommand( &_system ) );
    updateViews( E_SYSTEM_CHANGED_EVENT );
    setModified( true );

    coca::getEventManager().appendEventCallback( _eventFunction );
}

void CocaDocument::destroySystem()
{
    if ( !_system ) { return; }

    coca::getEventManager().removeEventCallback( _eventFunction );

    coca::ISystem* system = _system;
    _system = 0;
    updateViews( E_SYSTEM_CHANGED_EVENT );
    setModified( true );

    // Do this after the event to allow all referenced pointers to system nodes to be cleared.
    // This is actually obsolete since no reference pointers are used in the editor anymore,
    // but leave it like this anyway.
    execute( new DeleteSystemCommand( system ) );
}

bool CocaDocument::saveSystem( const std::string& filename )
{
    if ( !_system ) { return false; }
    bool ok = execute( new SaveSystemCommand( *_system, filename ) );
    if ( ok )
    {
        ::wxSetWorkingDirectory( ::wxPathOnly( filename ) );
        writeConfig( filename );
        setModified( false );
    }
    return ok;
}

bool CocaDocument::loadSystem( const std::string& filename )
{
    destroySystem();
    execute( new LoadSystemCommand( &_system, filename ) );
    setModified( false );
    if ( _system )
    {
        ::wxSetWorkingDirectory( ::wxPathOnly( filename ) );
        readConfig( filename );
        updateViews( E_SYSTEM_CHANGED_EVENT );

        coca::getEventManager().appendEventCallback( _eventFunction );
    }
    return ( _system != 0 );
}

const coca::INode* CocaDocument::readNode( const std::string& text, coca::IReader& reader )
{
    coca::INode* node = 0;
    execute( new ReadNodeCommand( text, reader, &node ) );
    updateViews();
    return node;
}

bool CocaDocument::editSystemDescription()
{
    if ( !_system ) { return false; }

    std::string description = _system->getDescription();
    if ( !EditorTools::enterSystemDescription( description ) ) { return false; }

    return setSystemDescription( description );
}

bool CocaDocument::setSystemDescription( const std::string& description )
{
    if ( !_system || description == _system->getDescription() ) { return false; }

    execute( new SetSystemDescriptionCommand( *_system, description ) );
    return updateViews();
}

bool CocaDocument::editSystemRoot()
{
    if ( !_system ) { return false; }

    std::string componentId;
    if ( EditorTools::selectNode( componentId ) == E_ID_CANCEL ) { return false; }

    return setSystemRoot( componentId, "" );
}

bool CocaDocument::setSystemRoot( const std::string& componentId, const std::string& name )
{
    if ( !_system ) { return false; }
    std::string enteredName = name;
    if ( componentId.empty() && !EditorTools::enterNodeName( enteredName ) ) { return false; }
    if ( !execute( new SetSystemRootCommand( *_system, componentId, enteredName ) ) ) { return false; }
    return updateViews();
}

bool CocaDocument::setSystemRoot( const coca::INode& node )
{
    if ( !_system ) { return false; }
    if ( !execute( new SetSystemRootCommand( *_system, const_cast<coca::INode&>( node ) ) ) ) { return false; }
    return updateViews();
}

bool CocaDocument::clearSystemRoot()
{
    if ( !_system ) { return false; }

    execute( new ClearSystemRootCommand( *_system ) );

    return updateViews();
}

bool CocaDocument::editNodeName( const coca::INode& node )
{
    std::string name = node.getName();
    if ( !EditorTools::enterNodeName( name ) ) { return false; }

    return setNodeName( node, name );
}

bool CocaDocument::setNodeName( const coca::INode& node, const std::string& name )
{
    if ( name == node.getName() ) { return false; }

    execute( new SetNodeNameCommand( const_cast<coca::INode&>( node ), name ) );
    return updateViews();
}

bool CocaDocument::enableNode( const coca::INode& node, bool enableChildren )
{
    if ( !enableChildren && node.isEnabled() ) { return false; }

    execute( new EnableNodeCommand( const_cast<coca::INode&>( node ), enableChildren ) );
    return updateViews();
}

bool CocaDocument::disableNode( const coca::INode& node, bool disableChildren )
{
    execute( new DisableNodeCommand( const_cast<coca::INode&>( node ), disableChildren ) );
    return updateViews();
}

bool CocaDocument::appendNode( const coca::INode& parent )
{
    if ( !parent.isCompositeNode() ) { return false; }

    std::string componentId;
    if ( EditorTools::selectNode( componentId ) == E_ID_CANCEL ) { return false; }

    return appendNode( const_cast<coca::INode&>( parent ), componentId, "" );
}

bool CocaDocument::appendNode( const coca::INode& parent, const std::string& componentId, const std::string& name )
{
    std::string enteredName = name;
    if ( componentId.empty() && enteredName.empty() && !EditorTools::enterNodeName( enteredName ) ) { return false; }
    if ( !execute( new AppendNodeCommand( const_cast<coca::INode&>( parent ), componentId, enteredName ) ) ) { return false; }
    return updateViews();
}

bool CocaDocument::appendNode( const coca::INode& parent, const coca::INode& node )
{
    if ( !execute( new AppendNodeCommand( const_cast<coca::INode&>( parent ), const_cast<coca::INode&>( node ) ) ) ) { return false; }
    return updateViews();
}

bool CocaDocument::removeNode( const coca::INode& node )
{
    if ( !node.getParent() )
    {
        if ( _system && &node == _system->getRoot() ) { return clearSystemRoot(); }
        else { return false; }
    }

    if ( !execute( new RemoveNodeCommand( const_cast<coca::INode&>( node ) ) ) ) { return false; }

    return updateViews();
}

bool CocaDocument::moveNode( const coca::INode& node, const coca::INode& newParent )
{
    if ( !newParent.isCompositeNode() || node.getParent() == 0
            || node.getParent() == &newParent ) { return false; }

    if ( !execute( new MoveNodeCommand( const_cast<coca::INode&>( node ),
                                        const_cast<coca::INode&>( newParent ) ) ) ) { return false; }

    return updateViews();
}

bool CocaDocument::moveNodeUp( const coca::INode& node )
{
    if ( !node.getParent() ) { return false; }
    if ( !execute( new MoveNodeUpCommand( const_cast<coca::INode&>( node ) ) ) ) { return false; }
    return updateViews();
}

bool CocaDocument::moveNodeDown( const coca::INode& node )
{
    if ( !node.getParent() ) { return false; }
    if ( !execute( new MoveNodeDownCommand( const_cast<coca::INode&>( node ) ) ) ) { return false; }
    return updateViews();
}

bool CocaDocument::connectAttributes( const coca::IAttribute& sourceAttribute, const coca::IAttribute& sinkAttribute )
{
    if ( !sourceAttribute.isOutputAttribute() || !sinkAttribute.isInputAttribute()
            || sourceAttribute.getTypeInfo() != sinkAttribute.getTypeInfo() ) { return false; }

    if ( !execute( new ConnectAttributesCommand( const_cast<coca::IAttribute&>( sourceAttribute ),
                   const_cast<coca::IAttribute&>( sinkAttribute ) ) ) ) { return false; }

    return updateViews();
}

bool CocaDocument::disconnectSink( const coca::IAttribute& attribute, const coca::IAttribute& sinkAttribute )
{
    return disconnectSource( sinkAttribute, attribute );
}

bool CocaDocument::disconnectSource( const coca::IAttribute& attribute, const coca::IAttribute& sourceAttribute )
{
    if ( !execute( new DisconnectSourceCommand( const_cast<coca::IAttribute&>( attribute ),
                   const_cast<coca::IAttribute&>( sourceAttribute ) ) ) ) { return false; }

    return updateViews();
}

bool CocaDocument::setAttributeValue( const coca::IAttribute& attribute, const std::string& stringValue )
{
    if ( !attribute.canBeSetAsString() ) { return false; }
    if ( !execute( new SetAttributeValueCommand( const_cast<coca::IAttribute&>( attribute ), stringValue ) ) ) { return false; }

    updateViews( E_ATTRIBUTE_VALUE_CHANGED_EVENT, attribute );
    setModified( true );
    return true;
}

// private //

bool CocaDocument::updateViews()
{
    bool hadEvents = !_events.empty();
    while ( !_events.empty() )
    {
        UpdateAllViews( 0, _events.top() );
        _events.pop();
    }
    if ( hadEvents ) { setModified( true ); }
    return hadEvents;
}

void CocaDocument::updateViews( ECocaDocumentEventType type )
{
    CocaDocumentEvent event( type );
    UpdateAllViews( 0, &event );
}

void CocaDocument::updateViews( ECocaDocumentEventType type, const coca::IAttribute& attribute )
{
    CocaDocumentEvent event( type, attribute );
    UpdateAllViews( 0, &event );
}

void CocaDocument::updateTickRateStatus()
{
    wxFrame* frame = wxGetApp().getFrame();
    if ( !frame ) { return; }

    std::ostringstream ss;
    ss << "clock tick rate: " << coca::getClock().getTickRate() << " Hz";
    frame->GetStatusBar()->SetStatusText( ss.str().c_str(), E_TICK_RATE_STATUS_FIELD );
}

wxConfigBase* CocaDocument::createConfig( const std::string filename ) const
{
    wxFileName path( filename );
    path.SetExt( "ini" );

    return new wxFileConfig( "cocaEditor", "Harm Boschloo", path.GetFullPath(), wxEmptyString,
                             wxCONFIG_USE_LOCAL_FILE );
}

void CocaDocument::readConfig( const std::string filename )
{
    if ( !wxFileExists( filename ) ) { return; }

    COCA_DEBUG_INFO( "CocaDocument::readConfig " << filename );

    std::auto_ptr<wxConfigBase> config( createConfig( filename ) );
    EditorFrame* frame = wxGetApp().getFrame();
    COCA_ASSERT( frame );

    // position
    int x = 25;
    int y = 25;
    config->Read( "frame/x", &x, x );
    config->Read( "frame/y", &y, y );

    // maximized
    bool maximized = false;
    config->Read( "frame/maximized", &maximized, maximized );
    frame->Maximize( maximized );

    if ( !maximized )
    {
        // size
        int width = 800;
        int height = 600;
        config->Read( "frame/width", &width, width );
        config->Read( "frame/height", &height, height );

        // check position
        int screenX = ::wxSystemSettings::GetMetric( ::wxSYS_SCREEN_X, frame );
        int screenY = ::wxSystemSettings::GetMetric( ::wxSYS_SCREEN_Y, frame );
        if ( x + width > screenX ) { x = screenX - width; }
        if ( y + height > screenY ) { y = screenY - height; }
        if ( x < 0 ) { x = 0; }
        if ( y < 0 ) { y = 0; }

        frame->SetSize( x, y, width, height );
    }

    // views
    wxList& views = GetViews();
    COCA_CHECK_EQUAL_SILENT( views.GetCount(), ( size_t )1 );
    int viewCount = 1;
    config->Read( "views/view_count", &viewCount, viewCount );
    viewCount -= views.GetCount();
    if ( viewCount < 0 ) { viewCount = 0; }
    if ( viewCount > 10 ) { viewCount = 10; }

    for ( int i = 0; i < viewCount; ++i )
    {
        GetDocumentManager()->CreateView( this );
    }

    nameViewPanes();

    wxList::iterator it;
    int i = 0;
    for ( it = views.begin(); it != views.end(); ++it )
    {
        ++i;
        CocaView* view = coca::staticCast<CocaView*>( *it );
        view->readConfig( *config, wxString( "view" ) << i );
    }

    // aui manager
    int version = 0;
    config->Read( "layout/version", &version, version );
    wxString layout;
    if ( version == CONFIG_LAYOUT_VERSION && config->Read( "layout/layout", &layout ) )
    {
        frame->getAuiManager().LoadPerspective( layout );
    }

    frame->getAuiManager().Update();
}

void CocaDocument::writeConfig( const std::string filename )
{
    if ( !wxFileExists( filename ) ) { return; }

    COCA_DEBUG_INFO( "CocaDocument::writeConfig " << filename );

    std::auto_ptr<wxConfigBase> config( createConfig( filename ) );
    EditorFrame* frame = wxGetApp().getFrame();
    COCA_ASSERT( frame );

    // clear current config
    config->DeleteAll();

    // frame
    if ( !frame->IsMaximized() )
    {
        config->Write( "frame/x", frame->GetPosition().x );
        config->Write( "frame/y", frame->GetPosition().y );
        config->Write( "frame/width", frame->GetSize().GetWidth() );
        config->Write( "frame/height", frame->GetSize().GetHeight() );
    }
    config->Write( "frame/maximized", frame->IsMaximized() );

    // views
    wxList& views = GetViews();
    config->Write( "views/view_count", static_cast<int>( views.GetCount() ) );
    nameViewPanes();

    wxList::iterator it;
    int i = 0;
    for ( it = views.begin(); it != views.end(); ++it )
    {
        ++i;
        CocaView* view = coca::staticCast<CocaView*>( *it );
        view->writeConfig( *config, wxString( "view" ) << i );
    }

    // aui manager
    config->Write( "layout/version", CONFIG_LAYOUT_VERSION );
    config->Write( "layout/layout", frame->getAuiManager().SavePerspective() );
}

// debug //

void CocaDocument::nameViewPanes()
{
    EditorFrame* frame = wxGetApp().getFrame();
    COCA_ASSERT( frame );

    wxList& views = GetViews();
    wxList::iterator it;
    int i = 0;
    for ( it = views.begin(); it != views.end(); ++it )
    {
        ++i;
        CocaView* view = coca::staticCast<CocaView*>( *it );
        if ( view->getPane() )
        {
            wxAuiPaneInfo& info = frame->getAuiManager().GetPane( view->getPane() );
            if ( info.IsOk() )
            {
                info.Name( wxString( "view" ) << i );
            }
        }
    }
}

bool CocaDocument::Close()
{
    COCA_DEBUG_INFO( "CocaDocument::Close " << this << "; " << m_documentViews.GetCount() );
    return wxDocument::Close();
}

bool CocaDocument::Save()
{
    COCA_DEBUG_INFO( "CocaDocument::Save " << this << "; " << m_documentViews.GetCount() );
    return wxDocument::Save();
}

bool CocaDocument::SaveAs()
{
    COCA_DEBUG_INFO( "CocaDocument::SaveAs " << this << "; " << m_documentViews.GetCount() );
    return wxDocument::SaveAs();
}

bool CocaDocument::Revert()
{
    COCA_DEBUG_INFO( "CocaDocument::Revert " << this << "; " << m_documentViews.GetCount() );
    return wxDocument::Revert();
}

bool CocaDocument::OnSaveDocument( const wxString& filename )
{
    COCA_DEBUG_INFO( "CocaDocument::OnSaveDocument " << this << "; " << filename.c_str() << "; "
                     << m_documentViews.GetCount() );
    return wxDocument::OnSaveDocument( filename );
}

bool CocaDocument::OnOpenDocument( const wxString& filename )
{
    COCA_DEBUG_INFO( "CocaDocument::OnOpenDocument " << this << "; " << filename.c_str() << "; "
                     << m_documentViews.GetCount() );
    return wxDocument::OnOpenDocument( filename );
}

bool CocaDocument::OnSaveModified()
{
    COCA_DEBUG_INFO( "CocaDocument::OnSaveModified " << this << "; " << m_documentViews.GetCount() );
    return wxDocument::OnSaveModified();
}

void CocaDocument::OnChangedViewList()
{
    COCA_DEBUG_INFO( "CocaDocument::OnChangedViewList " << this << "; " << m_documentViews.GetCount() );
    wxDocument::OnChangedViewList();
}

void CocaDocument::NotifyClosing()
{
    COCA_DEBUG_INFO( "CocaDocument::NotifyClosing count: " << this << "; " << m_documentViews.GetCount() );
    wxDocument::NotifyClosing();
}

bool CocaDocument::DeleteAllViews()
{
    COCA_DEBUG_INFO( "CocaDocument::DeleteAllViews " << this << "; " << m_documentViews.GetCount() );
    return wxDocument::DeleteAllViews();
}

bool CocaDocument::DeleteContents()
{
    COCA_DEBUG_INFO( "CocaDocument::DeleteContents " << this );
    return wxDocument::DeleteContents();
}

bool CocaDocument::AddView( wxView *view )
{
    COCA_DEBUG_INFO( "CocaDocument::AddView " << view << "; " << this << "; " << m_documentViews.GetCount() );
    return wxDocument::AddView( view );
}

bool CocaDocument::RemoveView( wxView *view )
{
    COCA_DEBUG_INFO( "CocaDocument::RemoveView " << view << "; " << this << "; " << m_documentViews.GetCount() );
    return wxDocument::RemoveView( view );
}

