// Copyright (C) 2007-2008 Harm Boschloo

#ifndef COCA_SYSTEM_EDITOR_VIEW_H_INCLUDED
#define COCA_SYSTEM_EDITOR_VIEW_H_INCLUDED

#include "CocaView.h"
#include <coca/INode.h>
#include <wx/treebase.h>
#include <wx/dnd.h>

class CocaViewPane;
class CocaSystemTree;
class CocaPropertyGrid;
class CocaDataObject;
class wxSplitterWindow;
class wxTreeEvent;
class wxPropertyGridEvent;
class wxPoint;

class CocaSystemEditorView : public CocaView
{
public:
    CocaSystemEditorView();
    ~CocaSystemEditorView();

    // wxView events
    bool OnCreate( wxDocument* document, long flags );
    void OnChangeFilename();
    bool OnClose( bool deleteWindow );

    // CocaView events
    void onDeactivated();
    void onActivated();

    // tree events
    void onTreeSelectionChanged( wxTreeEvent& event );
    void onTreeItemMenu( wxTreeEvent& event );
    void onTreeItemActivated( wxTreeEvent& event );
    void onBeginDrag( wxTreeEvent& event );
    void onEndDrag( wxTreeEvent& event );

    // property grid events
    void onPropertyChanging( wxPropertyGridEvent& event );
    void onPropertyChanged( wxPropertyGridEvent& event );
    void onPropertySelected( wxPropertyGridEvent& event );
    void onPropertyRightClick( wxPropertyGridEvent& event );
    void onPropertyDragging( wxPropertyGridEvent& event );

    // drag & drop events
    wxDragResult onTreeDrop( wxPoint point, wxDragResult def, const CocaDataObject* data );
    wxDragResult onGridDrop( wxPoint point, wxDragResult def, const CocaDataObject* data );

    // menu events
    void onEditNodeName( wxCommandEvent& event );
    void onEnableNode( wxCommandEvent& event );
    void onDisableNode( wxCommandEvent& event );
    void onMoveNodeUp( wxCommandEvent& event );
    void onMoveNodeDown( wxCommandEvent& event );
    void onAppendNode( wxCommandEvent& event );
    void onRemoveNode( wxCommandEvent& event );
    void onGoToSink( wxCommandEvent& event );
    void onRemoveSink( wxCommandEvent& event );
    void onGoToSource( wxCommandEvent& event );
    void onRemoveSource( wxCommandEvent& event );
    void onCopy( wxCommandEvent& event );
    void onPaste( wxCommandEvent& event );

    // ui update events
    void onUpdateEditNodeName( wxUpdateUIEvent& event );
    void onUpdateEnableNode( wxUpdateUIEvent& event );
    void onUpdateDisableNode( wxUpdateUIEvent& event );
    void onUpdateMoveNodeUp( wxUpdateUIEvent& event );
    void onUpdateMoveNodeDown( wxUpdateUIEvent& event );
    void onUpdateAppendNode( wxUpdateUIEvent& event );
    void onUpdateRemoveNode( wxUpdateUIEvent& event );
    void onUpdateGoToSink( wxUpdateUIEvent& event );
    void onUpdateRemoveSink( wxUpdateUIEvent& event );
    void onUpdateGoToSource( wxUpdateUIEvent& event );
    void onUpdateRemoveSource( wxUpdateUIEvent& event );
    void onUpdateCopy( wxUpdateUIEvent& event );
    void onUpdatePaste( wxUpdateUIEvent& event );

    // document events //
    bool processEvent( CocaDocumentEvent& event );
    CocaViewPane* getPane();

    void readConfig( wxConfigBase& config, wxString category );
    void writeConfig( wxConfigBase& config, wxString category );

private:
    const coca::INode* getCurrentNode() const;
    void updateCaptions() const;

    CocaViewPane* _pane;
    CocaSystemTree* _tree;
    CocaPropertyGrid* _pg;
    wxSplitterWindow* _splitter;
    const coca::INode* _currentNode;
    wxTreeItemId _currentNodeId;
    const coca::IAttribute* _currentAttribute;
    const coca::IAttribute* _currentSink;
    const coca::IAttribute* _currentSource;

    DECLARE_DYNAMIC_CLASS( CocaSystemEditorView )
    DECLARE_EVENT_TABLE()
};

#endif // COCA_SYSTEM_EDITOR_VIEW_H_INCLUDED

