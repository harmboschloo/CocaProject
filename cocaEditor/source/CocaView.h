// Copyright (C) 2007-2008 Harm Boschloo

#ifndef COCA_VIEW_H_INCLUDED
#define COCA_VIEW_H_INCLUDED

#include <wx/docview.h>

class CocaDocument;
class CocaDocumentEvent;
class CocaViewPane;
class wxConfigBase;

class CocaView : public wxView
{
public:
    CocaView();
    ~CocaView();

    virtual void Activate( bool activate );
    virtual void OnUpdate( wxView *sender, wxObject *hint );

    virtual bool processEvent( CocaDocumentEvent& event );
    virtual CocaViewPane* getPane();

    virtual void readConfig( wxConfigBase& config, wxString category );
    virtual void writeConfig( wxConfigBase& config, wxString category );

    CocaDocument& getDocument() const;
    bool isActive() const;

    virtual void onDeactivated() {};
    virtual void onActivated() {};

private:
    // required
    virtual void OnDraw( wxDC *dc );

public:
    // debug //
    virtual void OnActivateView( bool activate, wxView* activeView, wxView* deactiveView );
    virtual void OnChangeFilename();
    virtual void OnPrint( wxDC *dc, wxObject *info );
    virtual bool OnCreate( wxDocument *WXUNUSED( doc ), long WXUNUSED( flags ) );
    virtual bool Close( bool deleteWindow = true );
    virtual bool OnClose( bool deleteWindow );
    virtual void OnClosingDocument();

    DECLARE_DYNAMIC_CLASS( CocaView )
    DECLARE_EVENT_TABLE()
};

#endif // COCA_VIEW_H_INCLUDED

