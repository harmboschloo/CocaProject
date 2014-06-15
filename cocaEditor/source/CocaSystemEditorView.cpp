// Copyright (C) 2007-2008 Harm Boschloo

#include "CocaSystemEditorView.h"
#include "CocaDocument.h"
#include "CocaDocumentEvent.h"
#include "CocaViewPane.h"
#include "CocaSystemTree.h"
#include "CocaPropertyGrid.h"
#include "CocaDataObject.h"
#include "CocaDropTarget.h"
#include "EditorApplication.h"
#include "EditorFrame.h"
#include "EditorDefs.h"
#include "EditorTools.h"
#include "AttributeConnectionMenu.h"
#include <coca/INode.h>
#include <coca/IWriter.h>
#include <coca/IReader.h>
#include <coca/IFactory.h>
#include <coca/log/log.h>
#include <coca/utils/test.h>
#include <coca/utils/staticCast.h>
#include <wx/splitter.h>
#include <wx/confbase.h>
#include <wx/clipbrd.h>

IMPLEMENT_DYNAMIC_CLASS( CocaSystemEditorView, CocaView )

BEGIN_EVENT_TABLE( CocaSystemEditorView, CocaView )
    EVT_TREE_SEL_CHANGED( wxID_ANY, CocaSystemEditorView::onTreeSelectionChanged )
    EVT_TREE_ITEM_MENU( wxID_ANY, CocaSystemEditorView::onTreeItemMenu )
    EVT_TREE_ITEM_ACTIVATED( wxID_ANY, CocaSystemEditorView::onTreeItemActivated )
    EVT_TREE_BEGIN_DRAG( wxID_ANY, CocaSystemEditorView::onBeginDrag )
    EVT_TREE_END_DRAG( wxID_ANY, CocaSystemEditorView::onEndDrag )
    EVT_PG_CHANGING( wxID_ANY, CocaSystemEditorView::onPropertyChanging )
    EVT_PG_CHANGED( wxID_ANY, CocaSystemEditorView::onPropertyChanged )
    EVT_PG_SELECTED( wxID_ANY, CocaSystemEditorView::onPropertySelected )
    EVT_PG_RIGHT_CLICK( wxID_ANY, CocaSystemEditorView::onPropertyRightClick )
    EVT_PG_ITEM_DRAGGING( wxID_ANY, CocaSystemEditorView::onPropertyDragging )
    EVT_MENU( E_ID_EDIT_NODE_NAME, CocaSystemEditorView::onEditNodeName )
    EVT_MENU( E_ID_ENABLE_NODE, CocaSystemEditorView::onEnableNode )
    EVT_MENU( E_ID_DISABLE_NODE, CocaSystemEditorView::onDisableNode )
    EVT_MENU( E_ID_MOVE_NODE_UP, CocaSystemEditorView::onMoveNodeUp )
    EVT_MENU( E_ID_MOVE_NODE_DOWN, CocaSystemEditorView::onMoveNodeDown )
    EVT_MENU( E_ID_APPEND_NODE, CocaSystemEditorView::onAppendNode )
    EVT_MENU( E_ID_REMOVE_NODE, CocaSystemEditorView::onRemoveNode )
    EVT_MENU( E_ID_GO_TO_SINK, CocaSystemEditorView::onGoToSink )
    EVT_MENU( E_ID_REMOVE_SINK, CocaSystemEditorView::onRemoveSink )
    EVT_MENU( E_ID_GO_TO_SOURCE, CocaSystemEditorView::onGoToSource )
    EVT_MENU( E_ID_REMOVE_SOURCE, CocaSystemEditorView::onRemoveSource )
    EVT_MENU( wxID_COPY, CocaSystemEditorView::onCopy )
    EVT_MENU( wxID_PASTE, CocaSystemEditorView::onPaste )
    EVT_UPDATE_UI( E_ID_EDIT_NODE_NAME, CocaSystemEditorView::onUpdateEditNodeName )
    EVT_UPDATE_UI( E_ID_ENABLE_NODE, CocaSystemEditorView::onUpdateEnableNode )
    EVT_UPDATE_UI( E_ID_DISABLE_NODE, CocaSystemEditorView::onUpdateDisableNode )
    EVT_UPDATE_UI( E_ID_MOVE_NODE_UP, CocaSystemEditorView::onUpdateMoveNodeUp )
    EVT_UPDATE_UI( E_ID_MOVE_NODE_DOWN, CocaSystemEditorView::onUpdateMoveNodeDown )
    EVT_UPDATE_UI( E_ID_APPEND_NODE, CocaSystemEditorView::onUpdateAppendNode )
    EVT_UPDATE_UI( E_ID_REMOVE_NODE, CocaSystemEditorView::onUpdateRemoveNode )
    EVT_UPDATE_UI( E_ID_GO_TO_SINK, CocaSystemEditorView::onUpdateGoToSink )
    EVT_UPDATE_UI( E_ID_REMOVE_SINK, CocaSystemEditorView::onUpdateRemoveSink )
    EVT_UPDATE_UI( E_ID_GO_TO_SOURCE, CocaSystemEditorView::onUpdateGoToSource )
    EVT_UPDATE_UI( E_ID_REMOVE_SOURCE, CocaSystemEditorView::onUpdateRemoveSource )
    EVT_UPDATE_UI( wxID_COPY, CocaSystemEditorView::onUpdateCopy )
    EVT_UPDATE_UI( wxID_PASTE, CocaSystemEditorView::onUpdatePaste )
END_EVENT_TABLE()

CocaSystemEditorView::CocaSystemEditorView() :
        _pane( 0 ),
        _tree( 0 ),
        _pg( 0 ),
        _splitter( 0 ),
        _currentNode( 0 ),
        _currentAttribute( 0 ),
        _currentSink( 0 ),
        _currentSource( 0 )
{
    COCA_DEBUG_INFO( "CocaSystemEditorView Created " << this );
}

CocaSystemEditorView::~CocaSystemEditorView()
{
    COCA_DEBUG_INFO( "CocaSystemEditorView Destroyed " << this );
}

// wxView events //

bool CocaSystemEditorView::OnCreate( wxDocument* document, long flags )
{
    COCA_DEBUG_INFO( "CocaSystemEditorView OnCreate " << this );
    COCA_ASSERT_EQUAL( _pane, ( void* )0 );

    // create editor pane

    EditorFrame* frame = wxGetApp().getFrame();
    COCA_ASSERT( frame );

    _pane = new CocaViewPane( *this, frame );

    _splitter = new wxSplitterWindow( _pane );

    _tree = new CocaSystemTree( _splitter, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL|wxTR_DEFAULT_STYLE );
    _pg = new CocaPropertyGrid( _splitter, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );

    _splitter->SplitVertically( _tree, _pg, 0 );
    _splitter->SetMinimumPaneSize( 20 );

    _pane->SetSizer( new wxBoxSizer( wxVERTICAL ) );
    _pane->GetSizer()->Add( _splitter, 1, wxEXPAND, 5 );
    _pane->Layout();

    // attach editor pane

    wxAuiPaneInfo info;
    info.BestSize( 600, 300 ).MaximizeButton( true );

    if ( document->GetViews().GetCount() == 1 )
    {
        frame->getAuiManager().AddPane( _pane, info.Centre().CloseButton( false ) );
    }
    else
    {
        frame->getAuiManager().AddPane( _pane, info.Right().CloseButton( true ) );
    }

    // tree setup
    _tree->SetDropTarget( new CocaDropTarget( coca::makeFunction( *this, &CocaSystemEditorView::onTreeDrop ) ) );
    _tree->resetAll( getDocument().getSystem(), getDocument().getName( true ) );

    wxAcceleratorEntry entries[2];
    entries[0].Set( wxACCEL_CTRL, ( int )'C', wxID_COPY );
    entries[1].Set( wxACCEL_CTRL, ( int )'V', wxID_PASTE );
    wxAcceleratorTable accTable( 2, entries );
    _tree->SetAcceleratorTable( accTable );

    // property grid setup
    _pg->SetDropTarget( new CocaDropTarget( coca::makeFunction( *this, &CocaSystemEditorView::onGridDrop ) ) );

    updateCaptions();

    return true;
}

void CocaSystemEditorView::OnChangeFilename()
{
    COCA_ASSERT( _tree );

    _tree->setSystemName( getDocument().getName( true ) );
    updateCaptions();
}

bool CocaSystemEditorView::OnClose( bool )
{
    COCA_DEBUG_INFO( "CocaSystemEditorView::OnClose " << this );
    COCA_ASSERT( _pane );

    if ( getDocument().GetViews().GetCount() == 1 )
    {
        if ( !getDocument().Close() ) { return false; }
    }
    else if ( isActive() )
    {
        wxView* firstView = getDocument().GetFirstView();
        if ( firstView != this ) { firstView->Activate( true ); }
    }

    wxGetApp().getFrame()->getAuiManager().DetachPane( _pane );
    wxGetApp().getFrame()->getAuiManager().Update();
    _pane->Destroy();
    _pane = 0;

    return true;
}

// CocaView events //

void CocaSystemEditorView::onDeactivated()
{
    updateCaptions();
}

void CocaSystemEditorView::onActivated()
{
    updateCaptions();
}

// tree events //

void CocaSystemEditorView::onTreeSelectionChanged( wxTreeEvent& event )
{
    COCA_DEBUG_INFO( "CocaSystemEditorView::onTreeSelectionChanged " << this << "; " << _tree->getSelectedNode() );
    COCA_ASSERT( _pg );
    _pg->resetAll( _tree->getSelectedNode() );
}

void CocaSystemEditorView::onTreeItemMenu( wxTreeEvent& event )
{
    //COCA_DEBUG_INFO( "CocaSystemEditorView::onTreeItemMenu " << this );
    COCA_ASSERT( _pane );

    // tree does not select item on right click
    //_tree->SelectItem( event.GetItem() );

    _currentNodeId = event.GetItem();
    _currentNode = _tree->getNode( _currentNodeId );

    if ( _currentNode )
    {
        _tree->PopupMenu( &EditorTools::getNodeMenu() );
        _currentNode = 0;
        _currentNodeId = wxTreeItemId();
    }
    else if ( _tree->isSystem( event.GetItem() ) )
    {
        _tree->PopupMenu( &EditorTools::getSystemMenu() );
    }

    //COCA_DEBUG_INFO( "end CocaSystemEditorView::onTreeItemMenu " << this );
}

void CocaSystemEditorView::onTreeItemActivated( wxTreeEvent& event )
{
    //COCA_DEBUG_INFO( "CocaSystemEditorView::onTreeItemActivated " << this );
    COCA_ASSERT( _pane );

    const coca::INode* node = _tree->getNode( event.GetItem() );

    if ( node )
    {
        getDocument().editNodeName( *node );
    }
    else if ( _tree->isSystem( event.GetItem() ) )
    {
        getDocument().editSystemDescription();
    }

    //COCA_DEBUG_INFO( "end CocaSystemEditorView::onTreeItemActivated " << this );
}

void CocaSystemEditorView::onBeginDrag( wxTreeEvent& event )
{
    COCA_DEBUG_INFO( "CocaSystemEditorView::onBeginDrag " << this );
    //event.Allow(); // allows default tree dragging behavior, we don't want that here
    //event.Skip();

    const coca::INode* node = _tree->getNode( event.GetItem() );
    if ( !node ) { return; }

    CocaDataObject data( node );
    wxDropSource source( data, _tree );
    wxDragResult result = source.DoDragDrop();

    switch ( result )
    {
    case wxDragLink:
        COCA_DEBUG_INFO( "wxDragLink" );
        break;
    case wxDragCopy:
        COCA_DEBUG_INFO( "wxDragCopy" );
        break;
    case wxDragMove:
        COCA_DEBUG_INFO( "wxDragMove" );
        break;
    case wxDragCancel:
        COCA_DEBUG_INFO( "wxDragCancel" );
        break;
    case wxDragNone:
        COCA_DEBUG_INFO( "wxDragNone" );
        break;
    default:
        COCA_DEBUG_INFO( "default" );
        break;
    }
}

void CocaSystemEditorView::onEndDrag( wxTreeEvent& event )
{
    // only called when we allow onBeginDrag
    //COCA_DEBUG_INFO( "CocaSystemEditorView::onEndDrag " << this );
    event.Skip();
}

// property grid events //

void CocaSystemEditorView::onPropertyChanging( wxPropertyGridEvent& event )
{
    //COCA_DEBUG_INFO( "CocaSystemEditorView::onPropertyChanging " << this );

    //coca::IAttribute* attribute = _pg->getAttribute( event.GetProperty() );
    //if ( attribute && event.CanVeto() && !attribute->canBeSetAsString() ) { event.Veto(); }
    //else { event.Skip(); }
}

void CocaSystemEditorView::onPropertyChanged( wxPropertyGridEvent& event )
{
    //COCA_DEBUG_INFO( "CocaSystemEditorView::onPropertyChanged " << this );

    wxPGProperty* property = event.GetProperty();
    const coca::IAttribute* attribute = _pg->getAttribute( property );
    if ( !attribute ) { return; }

    std::string value = _pg->getValue( property );

    COCA_DEBUG_INFO( "property changed: " << value << "; " << property->GetType()
                     << "; editable: " << attribute->canBeSetAsString() );

    bool ok = getDocument().setAttributeValue( *attribute, value );
    if ( !ok ) { _pg->refreshValue( property, *attribute ); }
}

void CocaSystemEditorView::onPropertySelected( wxPropertyGridEvent& event )
{
    //COCA_DEBUG_INFO( "CocaSystemEditorView::onPropertySelected " << this );

    const coca::IAttribute* attribute = _pg->getAttribute( event.GetProperty() );
    if ( attribute )
    {
        COCA_DEBUG_INFO( "attribute selected, value: " << attribute->getAsString() );
    }
}

void CocaSystemEditorView::onPropertyRightClick( wxPropertyGridEvent& event )
{
    COCA_DEBUG_INFO( "CocaSystemEditorView::onPropertyRightClick " << this );

    COCA_ASSERT( _pane );

    wxPGProperty* property = event.GetProperty();
    if ( !property ) { return; }

    _currentAttribute = _pg->findAttribute( property );
    if ( !_currentAttribute ) { return; }

    _currentSink = _pg->getSink( property );
    _currentSource = _pg->getSource( property );

    if ( _currentSink )
    {
        _pane->PopupMenu( &EditorTools::getSinkMenu() );
    }
    else if ( _currentSource )
    {
        _pane->PopupMenu( &EditorTools::getSourceMenu() );
    }

    _currentAttribute = 0;
    _currentSink = 0;
    _currentSource = 0;
}

void CocaSystemEditorView::onPropertyDragging( wxPropertyGridEvent& event )
{
    COCA_DEBUG_INFO( "CocaSystemEditorView::onPropertyDragging " << this );

    const coca::IAttribute* attribute = _pg->getAttribute( event.GetProperty() );
    if ( !attribute ) { return; }
    COCA_ASSERT( _pg->getNode() );

    CocaDataObject data( _pg->getNode(), attribute );
    wxDropSource source( data, _pg );
    wxDragResult result = source.DoDragDrop();

    switch ( result )
    {
    case wxDragLink:
        COCA_DEBUG_INFO( "wxDragLink" );
        break;
    case wxDragCopy:
        COCA_DEBUG_INFO( "wxDragCopy" );
        break;
    case wxDragMove:
        COCA_DEBUG_INFO( "wxDragMove" );
        break;
    case wxDragCancel:
        COCA_DEBUG_INFO( "wxDragCancel" );
        break;
    case wxDragNone:
        COCA_DEBUG_INFO( "wxDragNone" );
        break;
    default:
        COCA_DEBUG_INFO( "default" );
        break;
    }
}

// drag & drop events //

wxDragResult CocaSystemEditorView::onTreeDrop( wxPoint point, wxDragResult def, const CocaDataObject* data )
{
    const coca::INode* dropNode = _tree->getNode( point );
    if ( !dropNode ) { return wxDragNone; }

    if ( !data ) { return def; } // test only

    COCA_DEBUG_INFO( "onTreeDrop: " << point.x << "," << point.y << "; " << def << "; " << data->getNode()
                     << "; " << data->getAttribute() );

    const coca::INode* dragNode = data->getNode();
    if ( !dragNode ) { return wxDragNone; }

    const coca::IAttribute* dragAttribute = data->getAttribute();

    if ( dragAttribute )
    {
        if ( !dragAttribute->isOutputAttribute() ) { return wxDragNone; }

        AttributeConnectionMenu menu( *dragNode, *dragAttribute, *dropNode );
        if ( !menu.hasConnections() || !_pg->PopupMenu( &menu ) ) { return wxDragNone; }

        const coca::IAttribute* dropAttribute = menu.getSelectedSink();
        if ( !dropAttribute || !dropAttribute->isInputAttribute()
                || dropAttribute->getTypeInfo() != dragAttribute->getTypeInfo() ) { return wxDragNone; }

        if ( !getDocument().connectAttributes( *dragAttribute, *dropAttribute ) ) { return wxDragError; }

        COCA_ASSERT_EQUAL( dragAttribute, dropAttribute->getSources()->back() );
        COCA_ASSERT_EQUAL( dragAttribute->getSinks()->back(), dropAttribute );

        return def;
    }
    else if ( dropNode->isCompositeNode() && dragNode->getParent() != 0
              && dragNode->getParent() != dropNode )
    {
        if ( !getDocument().moveNode( *dragNode, *dropNode ) ) { return wxDragError; }

        return def;
    }

    return wxDragNone;
}

wxDragResult CocaSystemEditorView::onGridDrop( wxPoint point, wxDragResult def, const CocaDataObject* data )
{
    wxPGProperty* property = _pg->getProperty( point );
    const coca::INode* dropNode = _pg->getNode();
    if ( !property || !dropNode ) { return wxDragNone; }

    const coca::IAttribute* dropAttribute = _pg->getAttribute( property );
    if ( !dropAttribute )
    {
        if ( _pg->isSourcesProperty( property ) || _pg->isSourceProperty( property ) )
        {
            dropAttribute = _pg->findAttribute( property );
        }
    }
    if ( !dropAttribute || !dropAttribute->isInputAttribute() ) { return wxDragNone; }

    if ( !data ) { return def; } // test only

    COCA_DEBUG_INFO( "onGridDrop: " << point.x << "," << point.y << "; " << def << "; " << data->getNode() );

    const coca::INode* dragNode = data->getNode();
    if ( !dragNode || dropNode == dragNode || dragNode->getAttributes().empty() ) { return wxDragNone; }

    const coca::IAttribute* dragAttribute = data->getAttribute();

    if ( !dragAttribute )
    {
        AttributeConnectionMenu menu( *dragNode, *dropNode, *dropAttribute );
        if ( !menu.hasConnections() || !_pg->PopupMenu( &menu ) ) { return wxDragNone; }

        dragAttribute = menu.getSelectedSource();
        if ( !dragAttribute ) { return wxDragNone; }
    }

    if ( !dragAttribute->isOutputAttribute() || dragAttribute == dropAttribute
            || dragAttribute->getTypeInfo() != dropAttribute->getTypeInfo() )
    {
        return wxDragNone;
    }

    if ( !getDocument().connectAttributes( *dragAttribute, *dropAttribute ) ) { return wxDragError; }

    COCA_ASSERT_EQUAL( dragAttribute, dropAttribute->getSources()->back() );
    COCA_ASSERT_EQUAL( dragAttribute->getSinks()->back(), dropAttribute );

    return def;
}

// menu events //

void CocaSystemEditorView::onEditNodeName( wxCommandEvent& event )
{
    COCA_DEBUG_INFO( "CocaSystemEditorView::onEditNodeName " << this << "; " << getCurrentNode() );
    const coca::INode* node = getCurrentNode();
    if ( node ) { getDocument().editNodeName( *node ); }
}

void CocaSystemEditorView::onEnableNode( wxCommandEvent& event )
{
    COCA_DEBUG_INFO( "CocaSystemEditorView::onEnableNode " << this << "; " << getCurrentNode() );
    const coca::INode* node = getCurrentNode();
    if ( node ) { getDocument().enableNode( *node, true ); }
}

void CocaSystemEditorView::onMoveNodeUp( wxCommandEvent& event )
{
    COCA_DEBUG_INFO( "CocaSystemEditorView::onMoveNodeUp " << this << "; " << getCurrentNode() );
    const coca::INode* node = getCurrentNode();
    if ( node ) { getDocument().moveNodeUp( *node ); }
}

void CocaSystemEditorView::onMoveNodeDown( wxCommandEvent& event )
{
    COCA_DEBUG_INFO( "CocaSystemEditorView::onMoveNodeDown " << this << "; " << getCurrentNode() );
    const coca::INode* node = getCurrentNode();
    if ( node ) { getDocument().moveNodeDown( *node ); }
}

void CocaSystemEditorView::onDisableNode( wxCommandEvent& event )
{
    COCA_DEBUG_INFO( "CocaSystemEditorView::onDisableNode " << this << "; " << getCurrentNode() );
    const coca::INode* node = getCurrentNode();
    if ( node ) { getDocument().disableNode( *node, true ); }
}

void CocaSystemEditorView::onAppendNode( wxCommandEvent& event )
{
    COCA_DEBUG_INFO( "CocaSystemEditorView::onAppendNode " << this << "; " << getCurrentNode() );
    const coca::INode* node = getCurrentNode();
    if ( node ) { getDocument().appendNode( *node ); }
}

void CocaSystemEditorView::onRemoveNode( wxCommandEvent& event )
{
    COCA_DEBUG_INFO( "CocaSystemEditorView::onRemoveNode " << this << "; " << getCurrentNode() );
    const coca::INode* node = getCurrentNode();
    if ( node && EditorTools::confirmRemove( *node ) == E_ID_YES )
    {
        getDocument().removeNode( *node );
    }
}

void CocaSystemEditorView::onGoToSink( wxCommandEvent& event )
{
    COCA_DEBUG_INFO( "CocaSystemEditorView::onGoToSink " << this << "; " << _currentAttribute );

    COCA_ASSERT( _currentSink );
    COCA_ASSERT( _currentSink->getOwner() );

    _tree->selectNode( *_currentSink->getOwner() );
    _pg->selectAttribute( *_currentSink );
}

void CocaSystemEditorView::onRemoveSink( wxCommandEvent& event )
{
    COCA_DEBUG_INFO( "CocaSystemEditorView::onRemoveSink " << this << "; " << _currentAttribute );

    COCA_ASSERT( _currentAttribute );
    COCA_ASSERT( _currentSink );

    getDocument().disconnectSink( *_currentAttribute, *_currentSink );
}

void CocaSystemEditorView::onGoToSource( wxCommandEvent& event )
{
    COCA_DEBUG_INFO( "CocaSystemEditorView::onGoToSource " << this << "; " << _currentAttribute );

    COCA_ASSERT( _currentSource );
    COCA_ASSERT( _currentSource->getOwner() );

    _tree->selectNode( *_currentSource->getOwner() );
    _pg->selectAttribute( *_currentSource );
}

void CocaSystemEditorView::onRemoveSource( wxCommandEvent& event )
{
    COCA_DEBUG_INFO( "CocaSystemEditorView::onRemoveSource " << this << "; " << _currentAttribute );

    COCA_ASSERT( _currentAttribute );
    COCA_ASSERT( _currentSource );

    getDocument().disconnectSource( *_currentAttribute, *_currentSource );
}

void CocaSystemEditorView::onCopy( wxCommandEvent& event )
{
    COCA_DEBUG_INFO( "CocaSystemEditorView::onCopy " << this << "; " << getCurrentNode() );

    const coca::INode* node = getCurrentNode();
    std::auto_ptr<coca::IWriter> writer( coca::getFactory().createWriter( "xml" ) );

    if ( node && writer.get() && wxTheClipboard->Open() )
    {
        std::ostringstream ss;
        if ( writer->write( *node, ss ) )
        {
            wxTheClipboard->SetData( new wxTextDataObject( ss.str() ) );
        }
        wxTheClipboard->Close();
    }
}

void CocaSystemEditorView::onPaste( wxCommandEvent& event )
{
    COCA_DEBUG_INFO( "CocaSystemEditorView::onPaste " << this << "; " << getCurrentNode() );

    const coca::INode* parentNode = 0;

    if ( !_tree->isSystemSelected() )
    {
        parentNode = getCurrentNode();
        if ( !parentNode ) { return; }
        if ( parentNode->isComponentNode() ) { parentNode = parentNode->getParent(); }
        if ( !parentNode ) { return; }
    }

    std::auto_ptr<coca::IReader> reader( coca::getFactory().createReader( "xml" ) );

    if ( reader.get() && wxTheClipboard->Open() )
    {
        wxTextDataObject data;
        wxTheClipboard->GetData( data );

        // use ReferencePointer just for exception safe type
        // but actually node should never be deleted in this thread....S
        coca::ReferencePointer<const coca::INode> node = getDocument().readNode( data.GetText().c_str(), *reader );

        wxTheClipboard->Close();

        if ( node )
        {
            if ( parentNode )
            {
                getDocument().appendNode( *parentNode, *node );
            }
            else
            {
                getDocument().setSystemRoot( *node );
            }
        }
    }
}

// ui update events //

void CocaSystemEditorView::onUpdateEditNodeName( wxUpdateUIEvent& event )
{
    event.Enable( getCurrentNode() != 0 );
}

void CocaSystemEditorView::onUpdateEnableNode( wxUpdateUIEvent& event )
{
    const coca::INode* node = getCurrentNode();
    if ( !node ) { event.Enable( false ); return; }

    wxString text( "enable" );

    if ( node->isEnabled() )
    {
        event.Enable( !node->getChildren().empty() );
    }
    else
    {
        text << " (-" << node->getDisabledCount() << ")";
        const coca::INode* parent = node->getParent();
        event.Enable( !parent || parent->getDisabledCount() < node->getDisabledCount() );
    }

    event.SetText( text );
}

void CocaSystemEditorView::onUpdateDisableNode( wxUpdateUIEvent& event )
{
    const coca::INode* node = getCurrentNode();
    if ( !node ) { event.Enable( false ); return; }

    wxString text( "disable" );

    if ( node->isDisabled() )
    {
        text << " (" << node->getDisabledCount() << ")";
    }
    event.SetText( text );
    event.Enable( true );
}

void CocaSystemEditorView::onUpdateMoveNodeUp( wxUpdateUIEvent& event )
{
    event.Enable( getCurrentNode() && getCurrentNode()->getParent() );
}

void CocaSystemEditorView::onUpdateMoveNodeDown( wxUpdateUIEvent& event )
{
    event.Enable( getCurrentNode() && getCurrentNode()->getParent() );
}

void CocaSystemEditorView::onUpdateAppendNode( wxUpdateUIEvent& event )
{
    const coca::INode* node = getCurrentNode();
    event.Enable( node && node->isCompositeNode() );
}

void CocaSystemEditorView::onUpdateGoToSink( wxUpdateUIEvent& event )
{
    event.Enable( _currentSink && _currentSink->getOwner() );
}

void CocaSystemEditorView::onUpdateRemoveSink( wxUpdateUIEvent& event )
{
    event.Enable( _currentAttribute && _currentSink );
}

void CocaSystemEditorView::onUpdateGoToSource( wxUpdateUIEvent& event )
{
    event.Enable( _currentSource && _currentSource->getOwner() );
}

void CocaSystemEditorView::onUpdateRemoveSource( wxUpdateUIEvent& event )
{
    event.Enable( _currentAttribute && _currentSource );
}

void CocaSystemEditorView::onUpdateRemoveNode( wxUpdateUIEvent& event )
{
    event.Enable( getCurrentNode() != 0 );
}

void CocaSystemEditorView::onUpdateCopy( wxUpdateUIEvent& event )
{
    event.Enable( _currentNode != 0 );
}

void CocaSystemEditorView::onUpdatePaste( wxUpdateUIEvent& event )
{
    event.Enable( _currentNodeId.IsOk() );
}

// document events //

bool CocaSystemEditorView::processEvent( CocaDocumentEvent& event )
{
    COCA_DEBUG_INFO( "CocaSystemEditorView::processEvent " << this << "; " << event.getType() );

    switch ( event.getType() )
    {
    case E_DOCUMENT_MODIFIED_CHANGED_EVENT:
        COCA_DEBUG_INFO( "E_DOCUMENT_MODIFIED_CHANGED_EVENT" );
        COCA_ASSERT( _tree );
        _tree->setSystemName( getDocument().getName( true ) );
        updateCaptions();
        break;
    case E_SYSTEM_CHANGED_EVENT:
    case coca::E_SYSTEM_ROOT_CHANGED_EVENT:
        COCA_DEBUG_INFO( "E_SYSTEM_CHANGED_EVENT/E_SYSTEM_ROOT_CHANGED_EVENT" );
        _tree->resetAll( getDocument().getSystem(), getDocument().getName( true ) );
        break;
    case coca::E_NODE_NAME_CHANGED_EVENT:
        COCA_DEBUG_INFO( "E_NODE_NAME_CHANGED_EVENT" );
        _tree->updateText( *event.getNode() );
        _pg->refreshAttributeNames( *event.getNode() );
        break;
    case coca::E_NODE_DISABLED_COUNT_CHANGED_EVENT:
        COCA_DEBUG_INFO( "E_NODE_DISABLED_COUNT_CHANGED_EVENT" );
        COCA_ASSERT( event.getNode() );
        if ( event.getNode() == _currentNode )
        {
            _tree->updateState( _currentNodeId );
        }
        else
        {
            _tree->updateState( *event.getNode() );
        }
        break;
    case coca::E_NODE_CHILD_APPENDED_EVENT:
        COCA_DEBUG_INFO( "E_NODE_CHILD_APPENDED_EVENT" );
        COCA_ASSERT( event.getNode2() );
        _tree->add( *event.getNode2(), isActive() );
        break;
    case coca::E_NODE_CHILD_REMOVED_EVENT:
        COCA_DEBUG_INFO( "E_NODE_CHILD_REMOVED_EVENT" );
        COCA_ASSERT( event.getNode2() );
        _tree->removeNode( event.getNode2() );
        if ( _pg->getNode() == event.getNode2() ) { _pg->resetAll( 0 ); }
        break;
    case coca::E_NODE_CHILD_MOVED_UP_EVENT:
        COCA_DEBUG_INFO( "E_NODE_CHILD_MOVED_UP_EVENT " << event.getNode2() << "; " << getCurrentNode() );
        COCA_ASSERT( event.getNode2() );
        _tree->moveUp( *event.getNode2() );
        break;
    case coca::E_NODE_CHILD_MOVED_DOWN_EVENT:
        COCA_DEBUG_INFO( "E_NODE_CHILD_MOVED_DOWN_EVENT " << event.getNode2() << "; " << getCurrentNode() );
        COCA_ASSERT( event.getNode2() );
        _tree->moveDown( *event.getNode2() );
        break;
    case coca::E_ATTRIBUTE_SINK_APPENDED_EVENT:
        COCA_DEBUG_INFO( "E_ATTRIBUTE_SINK_APPENDED_EVENT " << event.getAttribute() << "; " << event.getAttribute2() );
        if ( !_pg->getNode() ) { break; }
        COCA_ASSERT( event.getAttribute() );
        COCA_ASSERT( event.getAttribute2() );
        _pg->appendSink( *event.getAttribute(), *event.getAttribute2() );
        break;
    case coca::E_ATTRIBUTE_SINK_REMOVED_EVENT:
        COCA_DEBUG_INFO( "E_ATTRIBUTE_SINK_REMOVED_EVENT " << event.getAttribute() << "; " << event.getAttribute2() );
        if ( !_pg->getNode() ) { break; }
        COCA_ASSERT( event.getAttribute() );
        COCA_ASSERT( event.getAttribute2() );
        _pg->removeSink( *event.getAttribute(), event.getAttribute2() );
        break;
    case coca::E_ATTRIBUTE_SOURCE_APPENDED_EVENT:
        COCA_DEBUG_INFO( "E_ATTRIBUTE_SOURCE_APPENDED_EVENT " << event.getAttribute() << "; " << event.getAttribute2() );
        if ( !_pg->getNode() ) { break; }
        COCA_ASSERT( event.getAttribute() );
        COCA_ASSERT( event.getAttribute2() );
        _pg->appendSource( *event.getAttribute(), *event.getAttribute2() );
        break;
    case coca::E_ATTRIBUTE_SOURCE_REMOVED_EVENT:
        COCA_DEBUG_INFO( "E_ATTRIBUTE_SOURCE_REMOVED_EVENT " << event.getAttribute() << "; " << event.getAttribute2() );
        if ( !_pg->getNode() ) { break; }
        COCA_ASSERT( event.getAttribute() );
        COCA_ASSERT( event.getAttribute2() );
        _pg->removeSource( *event.getAttribute(), event.getAttribute2() );
        break;
    case E_ATTRIBUTE_VALUE_CHANGED_EVENT:
        COCA_DEBUG_INFO( "E_ATTRIBUTE_VALUE_CHANGED_EVENT" );
        if ( !_pg->getNode() ) { break; }
        _pg->refreshValue( *event.getAttribute() );
        break;
    default:
        return false;
        break;
    }

    return true;
}

CocaViewPane* CocaSystemEditorView::getPane()
{
    return _pane;
}

void CocaSystemEditorView::readConfig( wxConfigBase& config, wxString category )
{
    if ( !_splitter ) { return; }

    int sashPosition = 0;
    if ( config.Read( category + "/sash_position", &sashPosition ) )
    {
        _splitter->SetSashPosition( sashPosition );
    }
}

void CocaSystemEditorView::writeConfig( wxConfigBase& config, wxString category )
{
    config.Write( category + "/sash_position", _splitter->GetSashPosition() );
}

// private //

const coca::INode* CocaSystemEditorView::getCurrentNode() const
{
    return ( _currentNode ? _currentNode : _tree->getSelectedNode() );
}

void CocaSystemEditorView::updateCaptions() const
{
    if ( !_pane ) { return; }

    wxString name = getDocument().getName( true );
    wxString caption;
    if ( isActive() )
    {
        caption = ">";
        EditorTools::updateFrameTitle( getDocument().getName( true ) );
    }
    caption += name + " - system editor";
    wxGetApp().getFrame()->getAuiManager().GetPane( _pane ).Caption( caption );
    wxGetApp().getFrame()->getAuiManager().Update();
}
