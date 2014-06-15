// Copyright (C) 2007-2008 Harm Boschloo

#ifndef COCA_SYSTEM_TREE_H_INCLUDED
#define COCA_SYSTEM_TREE_H_INCLUDED

#include "EditorDefs.h"
#include <coca/INode.h>
#include <coca/ISystem.h>
#include <wx/treectrl.h>

class CocaDocument;

// ItemData
class ItemData : public wxTreeItemData
{
public:
    ItemData( const coca::INode& node );
    virtual ~ItemData();

    const coca::INode* getNode() const;

private:
    // We can not use a NodePtr here because nodes have to be deleted in the CocaThread!
    // It can happen that this pointer is not valid any more but we should get an event
    // about that!
    const coca::INode* _node;
};

class CocaSystemTree : public wxTreeCtrl
{
public:
    CocaSystemTree(
        wxWindow* parent,
        wxWindowID id = wxID_ANY,
        const wxPoint& pos = wxDefaultPosition,
        const wxSize& size = wxDefaultSize,
        long style = ( wxTR_DEFAULT_STYLE ),
        const wxValidator& validator = wxDefaultValidator,
        const wxString& name = wxTreeCtrlNameStr );
    virtual ~CocaSystemTree();

    // system //
    void resetAll( const coca::ISystem* system, const wxString& name = "" );
    void setSystemName( const wxString& name );
    bool isSystem( wxTreeItemId id ) const;
    bool isSystemSelected() const;
    bool selectSystem();

    // nodes //
    const coca::INode* getSelectedNode() const;
    const coca::INode* getNode( wxPoint point ) const;
    const coca::INode* getNode( wxTreeItemId id ) const;

    wxTreeItemId add( const coca::INode& node, bool select = false );
    bool removeNode( const void* nodePointer );
    bool moveUp( const coca::INode& node );
    bool moveDown( const coca::INode& node );
    bool selectNode( const coca::INode& node );
    void updateText( const coca::INode& node );
    void updateState( const coca::INode& node );

    // items //
    wxTreeItemId findId( const coca::INode& node ) const;
    wxTreeItemId findId( const coca::INode& node, wxTreeItemId parentId ) const;
    wxTreeItemId findId( const void* nodePointer ) const;
    wxTreeItemId findId( const void* nodePointer, wxTreeItemId parentId ) const;
    void updateState( wxTreeItemId id );

private:
    wxTreeItemId add( const coca::ISystem& system, wxString name );
    wxTreeItemId add( const coca::INode& node, wxTreeItemId parentId );
    void addChildren( const coca::INode& node, wxTreeItemId nodeId );

    DECLARE_EVENT_TABLE()
};

#endif // COCA_SYSTEM_TREE_H_INCLUDED
