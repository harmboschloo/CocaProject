// Copyright (C) 2007-2008 Harm Boschloo

#include "CocaSystemTree.h"
#include "EditorTools.h"
#include <coca/log/log.h>
#include <coca/utils/staticCast.h>

// ItemData //

ItemData::ItemData( const coca::INode& node ) :
        _node( &node )
{
}

ItemData::~ItemData()
{
}

const coca::INode* ItemData::getNode() const
{
    return _node;
}

// CocaSystemTree //

BEGIN_EVENT_TABLE( CocaSystemTree, wxTreeCtrl )
END_EVENT_TABLE()

CocaSystemTree::CocaSystemTree( wxWindow* parent, wxWindowID id, const wxPoint& pos,
                                const wxSize& size, long style, const wxValidator& validator,
                                const wxString& name ) :
        wxTreeCtrl( parent, id, pos, size, style, validator, name )
{
    // images
    SetImageList( &EditorTools::getIcons() );
}

CocaSystemTree::~CocaSystemTree()
{
    COCA_DEBUG_INFO( "CocaSystemTree::~CocaSystemTree " << this );
}

// system commands //

void CocaSystemTree::resetAll( const coca::ISystem* system, const wxString& name )
{
    COCA_DEBUG_INFO( "CocaSystemTree::resetAll " << system << "; " << name );
    DeleteAllItems();
    if ( system ) { add( *system, name ); }
}

void CocaSystemTree::setSystemName( const wxString& name )
{
    wxTreeItemId id = GetRootItem();
    if ( id.IsOk() ) { SetItemText( id, name ); }
}

bool CocaSystemTree::isSystem( wxTreeItemId id ) const
{
    return ( id.IsOk() && id == GetRootItem() );
}

bool CocaSystemTree::isSystemSelected() const
{
    wxTreeItemId id = GetRootItem();
    return ( id.IsOk() && id == GetSelection() );
}

bool CocaSystemTree::selectSystem()
{
    wxTreeItemId id = GetRootItem();
    if ( id.IsOk() )
    {
        SelectItem( id );
        EnsureVisible( id );
    }

    return id.IsOk();
}

// nodes //

const coca::INode* CocaSystemTree::getSelectedNode() const
{
    return getNode( GetSelection() );
}

const coca::INode* CocaSystemTree::getNode( wxPoint point ) const
{
    int flags = 0;
    return getNode( HitTest( point, flags ) );
}

const coca::INode* CocaSystemTree::getNode( wxTreeItemId id ) const
{
    if ( !id.IsOk() ) { return 0; }

    wxTreeItemData* data = GetItemData( id );
    if ( !data ) { return 0; }

    return coca::staticCast<ItemData*>( data )->getNode();
}

wxTreeItemId CocaSystemTree::add( const coca::INode& node, bool select )
{
    wxTreeItemId id;
    const coca::INode* parent = node.getParent();
    if ( !parent ) { return id; }

    id = findId( *parent );
    if ( !id.IsOk() ) { return id; }

    id = add( node, id );

    if ( select )
    {
        SelectItem( id );
        EnsureVisible( id );
    }

    return id;
}

bool CocaSystemTree::removeNode( const void* nodePointer )
{
    wxTreeItemId id = findId( nodePointer );
    if ( !id.IsOk() ) { return false; }

    wxTreeItemId parentId;
    if ( id == GetSelection() ) { parentId = GetItemParent( id ); }

    Delete( id );

    if ( parentId.IsOk() )
    {
        SelectItem( parentId );
        EnsureVisible( parentId );
    }

    return true;
}

bool CocaSystemTree::moveUp( const coca::INode& node )
{
    wxTreeItemId id = findId( node );
    if ( !id.IsOk() ) { return false; }

    wxTreeItemId parentId = GetItemParent( id );
    if ( !parentId.IsOk() ) { return false; }

    wxTreeItemId previousId = GetPrevSibling( id );
    if ( !previousId.IsOk() ) { return false; }

    // ready to move
    bool wasSelected = ( id == GetSelection() );

    Delete( id );

    previousId = GetPrevSibling( previousId );

    if ( !previousId.IsOk() )
    {
        id = PrependItem( parentId,
                          EditorTools::getName( node ), EditorTools::getImageIndex( node ),
                          -1, new ItemData( node ) );
    }
    else
    {
        id = InsertItem( parentId, previousId,
                         EditorTools::getName( node ), EditorTools::getImageIndex( node ),
                         -1, new ItemData( node ) );
    }

    COCA_ASSERT( id.IsOk() );

    SetItemTextColour( id, EditorTools::getTextColour( node ) );

    addChildren( node, id );

    if ( wasSelected ) { SelectItem( id ); }

    return true;
}

bool CocaSystemTree::moveDown( const coca::INode& node )
{
    wxTreeItemId id = findId( node );
    if ( !id.IsOk() ) { return false; }

    wxTreeItemId parentId = GetItemParent( id );
    if ( !parentId.IsOk() ) { return false; }

    wxTreeItemId nextId = GetNextSibling( id );
    if ( !nextId.IsOk() ) { return false; }

    // ready to move
    bool wasSelected = ( id == GetSelection() );

    Delete( id );
    id = InsertItem( parentId, nextId,
                         EditorTools::getName( node ), EditorTools::getImageIndex( node ),
                         -1, new ItemData( node ) );
    COCA_ASSERT( id.IsOk() );
    SetItemTextColour( id, EditorTools::getTextColour( node ) );

    addChildren( node, id );

    if ( wasSelected ) { SelectItem( id ); }

    return true;
}

bool CocaSystemTree::selectNode( const coca::INode& node )
{
    if ( &node == getSelectedNode() ) { return true; }

    wxTreeItemId id = findId( node );
    if ( id.IsOk() )
    {
        SelectItem( id );
        EnsureVisible( id );
    }

    return id.IsOk();
}

void CocaSystemTree::updateText( const coca::INode& node )
{
    wxTreeItemId id = findId( node );
    if ( id.IsOk() ) { SetItemText( id, EditorTools::getName( node ) ); }
}

void CocaSystemTree::updateState( const coca::INode& node )
{
    updateState( findId( node ) );
}

// items //

wxTreeItemId CocaSystemTree::findId( const coca::INode& node ) const
{
    return findId( node, GetRootItem() );
}

wxTreeItemId CocaSystemTree::findId( const coca::INode& node, wxTreeItemId id ) const
{
    if ( !id.IsOk() ) { return id; }

    if ( &node == getNode( id ) ) { return id; }

    wxTreeItemIdValue cookie = 0;
    id = GetFirstChild( id, cookie );
    for ( ; id.IsOk(); id = GetNextSibling( id ) )
    {
        wxTreeItemId foundId = findId( node, id );
        if ( foundId.IsOk() ) { return foundId; }
    }

    return id; // is invalid
}

wxTreeItemId CocaSystemTree::findId( const void* nodePointer ) const
{
    return findId( nodePointer, GetRootItem() );
}

wxTreeItemId CocaSystemTree::findId( const void* nodePointer, wxTreeItemId id ) const
{
    if ( !id.IsOk() ) { return id; }

    if ( nodePointer == getNode( id ) ) { return id; }

    wxTreeItemIdValue cookie = 0;
    id = GetFirstChild( id, cookie );
    for ( ; id.IsOk(); id = GetNextSibling( id ) )
    {
        wxTreeItemId foundId = findId( nodePointer, id );
        if ( foundId.IsOk() ) { return foundId; }
    }

    return id; // is invalid
}

void CocaSystemTree::updateState( wxTreeItemId id )
{
    if ( !id.IsOk() ) { return; }

    const coca::INode* node = getNode( id );
    if ( !node ) { return; }

    SetItemImage( id, EditorTools::getImageIndex( *node ) );
    SetItemTextColour( id, EditorTools::getTextColour( *node ) );

    /*if ( updateChildren )
    {
        wxTreeItemIdValue cookie = 0;
        id = GetFirstChild( id, cookie );
        for ( ; id.IsOk(); id = GetNextSibling( id ) )
        {
            updateState( id, updateChildren );
        }
    }*/
}

// private //

wxTreeItemId CocaSystemTree::add( const coca::ISystem& system, wxString name )
{
    wxTreeItemId id = AddRoot( name, E_COCA_IMAGE );
    if ( system.getRoot() ) { add( *system.getRoot(), id ); }

    Expand( id );
    EnsureVisible( id );
    SelectItem( id );

    return id;
}

wxTreeItemId CocaSystemTree::add( const coca::INode& node, wxTreeItemId parentId )
{
    if ( !parentId.IsOk() ) { return parentId; }

    wxTreeItemId id = AppendItem( parentId, EditorTools::getName( node ), EditorTools::getImageIndex( node ),
                                  -1, new ItemData( node ) );
    SetItemTextColour( id, EditorTools::getTextColour( node ) );

    addChildren( node, id );

    return id;
}

void CocaSystemTree::addChildren( const coca::INode& node, wxTreeItemId nodeId )
{
    const coca::INode::NodeVector& children = node.getChildren();
    coca::INode::NodeVector::const_iterator it;
    for ( it = children.begin(); it != children.end(); ++it )
    {
        add( **it, nodeId );
    }

    Expand( nodeId ); // fixes a problem with [+] icon not showing properly sometimes
}

