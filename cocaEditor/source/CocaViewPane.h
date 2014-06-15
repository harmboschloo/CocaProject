// Copyright (C) 2007-2008 Harm Boschloo

#ifndef COCA_VIEW_PANE_H_INCLUDED
#define COCA_VIEW_PANE_H_INCLUDED

#include <wx/panel.h>
#include <wx/docview.h>
#include <wx/aui/aui.h>

class CocaView;
class CocaDocument;

class CocaViewPane : public wxPanel
{
public:
    CocaViewPane( CocaView& view,
                  wxWindow* parent,
                  wxWindowID id = wxID_ANY,
                  const wxPoint& pos = wxDefaultPosition,
                  const wxSize& size = wxDefaultSize,
                  long type = wxTAB_TRAVERSAL | wxNO_BORDER,
                  const wxString& name = wxPanelNameStr );
    virtual ~CocaViewPane();

    // Extend event processing to search the view's event table
    virtual bool ProcessEvent( wxEvent& event );

    void onIdle( wxIdleEvent& event );
    void onChildFocus( wxChildFocusEvent& event );

    CocaDocument& getDocument() const;
    CocaView& getView() const;
    //bool Destroy() { m_childView = ( wxView * )NULL; return wxFrame::Destroy(); }

protected:
    CocaView* _view;

private:
    DECLARE_CLASS( CocaViewPane )
    DECLARE_NO_COPY_CLASS( CocaViewPane )
    DECLARE_EVENT_TABLE()
};

#endif // COCA_VIEW_PANE_H_INCLUDED

