// Copyright (C) 2007-2008 Harm Boschloo

#ifndef COCA_DOCUMENT_H_INCLUDED
#define COCA_DOCUMENT_H_INCLUDED

#include "CommandPtr.h"
#include "ECocaDocumentEventType.h"
#include <coca/ISystem.h>
#include <coca/IEventManager.h>
#include <coca/IReader.h>
#include <coca/attributes/IAttribute.h>
#include <coca/utils/Observable.h>
#include <wx/docview.h>
#include <wx/timer.h>
#include <queue>

class CocaDocumentEvent;
class wxTimerEvent;
class wxConfigBase;

class CocaDocument: public wxDocument
{
public:
    CocaDocument();
    ~CocaDocument();

    // wxDocument overwrites //
    bool OnNewDocument();
    bool DoSaveDocument( const wxString& filename );
    bool DoOpenDocument( const wxString& filename );
    bool OnCloseDocument();

    // wx events //
    void onIdle( wxIdleEvent& event );
    void onTimer( wxTimerEvent& event );
    void onEditSystemDescription( wxCommandEvent& event );
    void onEditSystemRoot( wxCommandEvent& event );

    // coca events
    void onCocaEvent( const coca::IEvent& event );

    // queries //
    const coca::ISystem* getSystem() const;
    wxString getName( bool prependModified = false ) const;

    // commands //
    void post( const CommandPtr& command );
    void executeCommands();
    bool execute( const CommandPtr& command );

    void setModified( bool modified = true );
    void updateClock();
    void resetClock();
    void createSystem();
    void destroySystem();
    bool saveSystem( const std::string& filename );
    bool loadSystem( const std::string& filename );
    const coca::INode* readNode( const std::string& text, coca::IReader& reader );
    bool editSystemDescription();
    bool setSystemDescription( const std::string& description );
    bool editSystemRoot();
    bool setSystemRoot( const std::string& componentId, const std::string& name );
    bool setSystemRoot( const coca::INode& node );
    bool clearSystemRoot();
    bool editNodeName( const coca::INode& node );
    bool setNodeName( const coca::INode& node, const std::string& name );
    bool enableNode( const coca::INode& node, bool enableChildren );
    bool disableNode( const coca::INode& node, bool disableChildren );
    bool appendNode( const coca::INode& parent );
    bool appendNode( const coca::INode& parent, const std::string& componentId, const std::string& name );
    bool appendNode( const coca::INode& parent, const coca::INode& node );
    bool removeNode( const coca::INode& node );
    bool moveNode( const coca::INode& node, const coca::INode& newParent );
    bool moveNodeUp( const coca::INode& node );
    bool moveNodeDown( const coca::INode& node );
    bool connectAttributes( const coca::IAttribute& sourceAttribute, const coca::IAttribute& sinkAttribute );
    bool disconnectSink( const coca::IAttribute& attribute, const coca::IAttribute& sinkAttribute );
    bool disconnectSource( const coca::IAttribute& attribute, const coca::IAttribute& sourceAttribute );
    bool setAttributeValue( const coca::IAttribute& attribute, const std::string& stringValue );

private:
    typedef coca::ReferencePointer<CocaDocumentEvent> EventPtr;

    class EventComparator
    {
    public:
        bool operator()( const EventPtr& event1, const EventPtr& event2 ) const;
        int getPriority( const EventPtr& event ) const;
    };

    static const int CONFIG_LAYOUT_VERSION;

    bool updateViews();
    void updateViews( ECocaDocumentEventType type );
    void updateViews( ECocaDocumentEventType type, const coca::IAttribute& attribute );

    //void updateFrameTitle( bool useDocumentName );
    void updateTickRateStatus();
    wxConfigBase* createConfig( const std::string filename ) const;
    void readConfig( const std::string filename );
    void writeConfig( const std::string filename );
    void nameViewPanes();

    coca::ISystem* _system;
    coca::IEventManager::EventCallback _eventFunction;
    std::priority_queue<EventPtr,std::vector<EventPtr>,EventComparator> _events;
    wxTimer _timer;

    DECLARE_DYNAMIC_CLASS( CocaDocument )
    DECLARE_EVENT_TABLE()

public:
    // debug //
    virtual bool Close();
    virtual bool Save();
    virtual bool SaveAs();
    virtual bool Revert();
    virtual bool OnSaveDocument( const wxString& filename );
    virtual bool OnOpenDocument( const wxString& filename );
    virtual bool OnSaveModified();
    virtual void OnChangedViewList();
    virtual void NotifyClosing();
    virtual bool DeleteAllViews();
    virtual bool DeleteContents();
    virtual bool AddView( wxView *view );
    virtual bool RemoveView( wxView *view );
};

#endif // COCA_DOCUMENT_H_INCLUDED

