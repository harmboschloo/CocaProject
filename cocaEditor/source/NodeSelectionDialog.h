// Copyright (C) 2007-2008 Harm Boschloo

#ifndef NODE_SELECTION_DIALOG_H_INCLUDED
#define NODE_SELECTION_DIALOG_H_INCLUDED

#include "EditorDefs.h"
#include <wx/dialog.h>
#include <wx/treectrl.h>

class wxTreeCtrl;
class wxTreeEvent;

class NodeSelectionDialog : public wxDialog
{
    DECLARE_EVENT_TABLE()

public:
    NodeSelectionDialog(
        wxWindow* parent,
        wxWindowID id,
        const wxString& title,
        const wxPoint& pos = wxDefaultPosition,
        const wxSize& size = wxDefaultSize,
        long style = wxDEFAULT_DIALOG_STYLE | wxRESIZE_BORDER,
        const wxString& name = wxDialogNameStr );
    virtual ~NodeSelectionDialog();

    void onItemActivated( wxTreeEvent& event );
    void onUpdateOk( wxUpdateUIEvent& event );

    void reset();
    bool isCompositeNodeSelected() const;
    bool isComponentNodeSelected() const;
    wxString getComponentId() const;

private:
    wxTreeCtrl* _nodeTree;
    wxTreeItemId _compositeId;
};

#endif // NODE_SELECTION_DIALOG_H_INCLUDED

