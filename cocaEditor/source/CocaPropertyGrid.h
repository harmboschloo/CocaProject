// Copyright (C) 2007-2008 Harm Boschloo

#ifndef COCA_PROPERTY_GRID_H_INCLUDED
#define COCA_PROPERTY_GRID_H_INCLUDED

#include <wx/propgrid/propgrid.h>
#include <wx/timer.h>
#include <coca/INode.h>
#include <coca/attributes/IAttribute.h>
#include <map>

class wxTimerEvent;

class CocaPropertyGrid : public wxPropertyGrid
{
public:
    typedef std::map<const coca::INode*,wxString> StateMap;

    CocaPropertyGrid(
        wxWindow* parent,
        wxWindowID id = wxID_ANY,
        const wxPoint& pos = wxDefaultPosition,
        const wxSize& size = wxDefaultSize,
        long style = wxPG_DEFAULT_STYLE,
        const wxChar* name = wxPropertyGridNameStr );
    ~CocaPropertyGrid();

    void onChildFocus( wxChildFocusEvent& event );
    void onTimer( wxTimerEvent& event );

    void resetAll( const coca::INode* node );
    const coca::INode* getNode() const;

    void selectAttribute( const coca::IAttribute& attribute );

    void refreshValue( const coca::IAttribute& attribute );
    void refreshValue( wxPGProperty* property, const coca::IAttribute& attribute );
    void updateValues();
    void appendSink( const coca::IAttribute& attribute, const coca::IAttribute& sinkAttribute );
    void removeSink( const coca::IAttribute& attribute, const void* sinkAttribute );
    void appendSource( const coca::IAttribute& attribute, const coca::IAttribute& sourceAttribute );
    void removeSource( const coca::IAttribute& attribute, const void* sourceAttribute );
    void refreshSource( const coca::IAttribute& attribute );
    void refreshSource( const coca::IAttribute& attribute, wxPGProperty* property );
    void refreshAttributeNames( const coca::INode& owner );
    void refreshAttributeNames( const coca::INode& owner, wxPGProperty* attributeProperty );

    wxPGProperty* getProperty( wxPoint point ) const;
    wxPGProperty* findProperty( const coca::IAttribute& attribute );

    bool isAttributeProperty( wxPGProperty* property ) const;
    bool isSinksProperty( wxPGProperty* property ) const;
    bool isSinkProperty( wxPGProperty* property ) const;
    bool isSourcesProperty( wxPGProperty* property ) const;
    bool isSourceProperty( wxPGProperty* property ) const;

    const coca::IAttribute* getAttribute( wxPGProperty* property ) const;
    const coca::IAttribute::AttributeVector* getSinks( wxPGProperty* property ) const;
    const coca::IAttribute* getSink( wxPGProperty* property ) const;
    const coca::IAttribute::AttributeVector* getSources( wxPGProperty* property ) const;
    const coca::IAttribute* getSource( wxPGProperty* property ) const;

    const coca::IAttribute* findAttribute( wxPGProperty* property ) const;

    std::string getValue( wxPGProperty* property ) const;
    bool setValue( wxPGProperty* property, const std::string& value ) const;

    const StateMap& getStates() const;
    wxString findState( const coca::INode* node ) const;
    void storeState( const coca::INode* node, const wxString& state );

private:
    void add( const coca::IAttribute& attribute, const std::string& name );
    void addSinks( const coca::IAttribute& attribute, wxPGProperty* sinksProperty );
    void addSink( const coca::IAttribute& attribute, wxPGProperty* sinksProperty );
    void addSources( const coca::IAttribute& attribute, wxPGProperty* sourcesProperty );
    void addSource( const coca::IAttribute& attribute, wxPGProperty* sourcesProperty );
    void refreshChildCount( wxPGProperty* property, bool refreshChildIndices = false );
    wxPGProperty* createProperty( const coca::IAttribute& attribute, const std::string& name );
    void setEditable( wxPGProperty* property, bool editable );
    wxString getPointerString( const void* pointer );
    void refreshAttributeName( const coca::INode& owner, wxPGProperty* attributeProperty );


    const coca::INode* _node;
    std::map<const coca::INode*,wxString> _states;
    wxTimer _timer;

    DECLARE_EVENT_TABLE()
};

#endif // COCA_PROPERTY_GRID_H_INCLUDED
