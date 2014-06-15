// Copyright (C) 2007-2008 Harm Boschloo

#include "NodeSelectionDialog.h"
#include "EditorTools.h"
#include "ScopedWXBusyCursor.h"
#include <coca/IFactory.h>
#include <wx/sizer.h>

BEGIN_EVENT_TABLE( NodeSelectionDialog, wxDialog )
    EVT_TREE_ITEM_ACTIVATED( wxID_ANY, NodeSelectionDialog::onItemActivated )
    EVT_UPDATE_UI( wxID_OK, NodeSelectionDialog::onUpdateOk )
END_EVENT_TABLE()

NodeSelectionDialog::NodeSelectionDialog( wxWindow* parent, wxWindowID id,
        const wxString& title, const wxPoint& pos, const wxSize& size, long style, const wxString& name ) :
        wxDialog( parent, id, title, pos, size, style, name ),
        _nodeTree( 0 )
{
    ScopedWXBusyCursor _busy;

    wxBoxSizer* sizer = new wxBoxSizer( wxVERTICAL );

    // node tree
    _nodeTree = new wxTreeCtrl( this );
    _nodeTree->SetImageList( &EditorTools::getIcons() );
    reset();

    sizer->Add( _nodeTree, wxSizerFlags().Expand().DoubleBorder().Proportion( 1 ) );
    //sizer->Add( _nodeTree, 1, wxALL|wxEXPAND, 5 );

    // buttons
    wxSizer* buttonSizer = CreateButtonSizer( wxOK | wxCANCEL );
    sizer->Add( buttonSizer, wxSizerFlags().Centre().DoubleBorder().Proportion( 0 ) );

    SetSizer( sizer );
    SetSize( wxSize( 400, 600 ) );
    Layout();

    _nodeTree->SetFocus();
}

NodeSelectionDialog::~NodeSelectionDialog()
{
}

void NodeSelectionDialog::onItemActivated( wxTreeEvent& event )
{
    wxTreeItemId id = event.GetItem();
    if ( event.GetItem() == _nodeTree->GetRootItem() )
    {
        EndModal( wxID_CANCEL );
    }
    else
    {
        EndModal( wxID_OK );
    }
}

void NodeSelectionDialog::onUpdateOk( wxUpdateUIEvent& event )
{
    wxTreeItemId id = _nodeTree->GetSelection();
    event.Enable( id.IsOk() && id != _nodeTree->GetRootItem() );
}

void NodeSelectionDialog::reset()
{
    _nodeTree->DeleteAllItems();

    wxTreeItemId rootId = _nodeTree->AddRoot( "nodes", E_COCA_IMAGE );

    _compositeId = _nodeTree->AppendItem( rootId, "composite node", E_COMPOSITE_IMAGE );

    const coca::IFactory::ComponentRegistry::CreatorMap& registrations =
        coca::getFactory().getComponentRegistry().getRegistrations();

    coca::IFactory::ComponentRegistry::CreatorMap::const_iterator it;
    for ( it = registrations.begin(); it != registrations.end(); ++it )
    {
        _nodeTree->AppendItem( rootId, it->first, E_COMPONENT_IMAGE );
    }

    _nodeTree->Expand( rootId );
}

bool NodeSelectionDialog::isCompositeNodeSelected() const
{
    wxTreeItemId id = _nodeTree->GetSelection();
    return ( id.IsOk() && id == _compositeId );
}

bool NodeSelectionDialog::isComponentNodeSelected() const
{
    wxTreeItemId id = _nodeTree->GetSelection();
    return ( id.IsOk() && id != _compositeId && id != _nodeTree->GetRootItem() );
}

wxString NodeSelectionDialog::getComponentId() const
{
    if ( isComponentNodeSelected() ) { return _nodeTree->GetItemText( _nodeTree->GetSelection() ); }
    else {  return wxEmptyString; }
}
