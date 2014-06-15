// Copyright (C) 2007-2008 Harm Boschloo

#ifndef EDITOR_FRAME_H_INCLUDED
#define EDITOR_FRAME_H_INCLUDED

#include <wx/frame.h>
#include <wx/docview.h>
#include <wx/aui/aui.h>

class EditorFrame : public wxDocParentFrame
{
    DECLARE_EVENT_TABLE()

public:
    EditorFrame(
        wxFrame* parent,
        int id,
        const wxString& title,
        const wxPoint& pos = wxDefaultPosition,
        const wxSize& size = wxDefaultSize,
        long style = wxDEFAULT_FRAME_STYLE );
    ~EditorFrame();

    void onUpdateViewWindowToggle( wxUpdateUIEvent& event );
    void onUpdateDefault( wxUpdateUIEvent& event );
    void onViewWindowToggle( wxCommandEvent& event );

    wxAuiManager& getAuiManager();

private:
    void createMenus();
    void createToolbars();
    void createWindows();

    wxAuiManager _auiManager;
};

#endif // EDITOR_FRAME_H_INCLUDED
