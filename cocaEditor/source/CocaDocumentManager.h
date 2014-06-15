// Copyright (C) 2007-2008 Harm Boschloo

#ifndef COCA_DOCUMENT_MANAGER_H_INCLUDED
#define COCA_DOCUMENT_MANAGER_H_INCLUDED

#include <wx/docview.h>

class wxAuiManagerEvent;

class CocaDocumentManager : public wxDocManager
{
public:
    CocaDocumentManager();
    ~CocaDocumentManager();

    void onUpdateAddSystemEditorView( wxUpdateUIEvent& event );

    void onAddSystemEditorView( wxCommandEvent& event );
    void onAuiPaneClose( wxAuiManagerEvent& event );

private:
    DECLARE_EVENT_TABLE()
};

#endif // COCA_DOCUMENT_MANAGER_H_INCLUDED
