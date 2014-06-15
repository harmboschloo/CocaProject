// Copyright (C) 2007-2008 Harm Boschloo

#include "CocaPropertyGrid.h"
#include "EditorTools.h"
#include <coca/log/log.h>
#include <coca/utils/type.h>
#include <wx/timer.h>

// ColumnType

enum ColumnType
{
    E_LABEL_COLUMN,
    E_VALUE_COLUMN,
    E_INFO_COLUMN,
    E_NUMBER_OF_COLUMNS
};

// CocaPropertyGrid //

BEGIN_EVENT_TABLE( CocaPropertyGrid, wxPropertyGrid )
    EVT_CHILD_FOCUS( CocaPropertyGrid::onChildFocus )
    EVT_TIMER( wxID_ANY, CocaPropertyGrid::onTimer )
END_EVENT_TABLE()

CocaPropertyGrid::CocaPropertyGrid( wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxSize& size,
                                    long style, const wxChar* name ) :
        wxPropertyGrid( parent, id, pos, size, style, name ),
        _node( 0 ),
        _timer( this )
{
    COCA_DEBUG_INFO( "CocaPropertyGrid::CocaPropertyGrid " << this );

    SetColumnCount( E_NUMBER_OF_COLUMNS );
    _timer.Start( 500 );
}

CocaPropertyGrid::~CocaPropertyGrid()
{
    COCA_DEBUG_INFO( "CocaPropertyGrid::~CocaPropertyGrid " << this );
}

void CocaPropertyGrid::onChildFocus( wxChildFocusEvent& event )
{
    //COCA_DEBUG_INFO( "CocaPropertyGrid::onChildFocus "  << this << "; " << &event << "; "
    //                 << GetPanel() << "; " << event.GetWindow() );
    // wxPropertyGrid block further propagation of the event so we process it here
    if ( GetParent() ) { GetParent()->ProcessEvent( event ); }
}

void CocaPropertyGrid::onTimer( wxTimerEvent& event )
{
    updateValues();
    RefreshGrid();
}

void CocaPropertyGrid::resetAll( const coca::INode* node )
{
    if ( _node ) { storeState( _node, SaveEditableState() ); }

    Freeze();

    Clear();

    _node = node;

    if ( _node )
    {

        const coca::INode::AttributeMap& attributes = _node->getAttributes();
        coca::INode::AttributeMap::const_iterator it;
        for ( it = attributes.begin(); it != attributes.end(); ++it )
        {
            add( *it->second, it->first );
        }

        FitColumns();

        StateMap::const_iterator it2 = _states.find( _node );
        if ( it2 != _states.end() ) { RestoreEditableState( it2->second ); }
    }

    Thaw();
}

const coca::INode* CocaPropertyGrid::getNode() const
{
    return _node;
}

void CocaPropertyGrid::selectAttribute( const coca::IAttribute& attribute )
{
    wxPGProperty* property = findProperty( attribute );
    if ( property ) { SelectProperty( property ); }
}

void CocaPropertyGrid::refreshValue( const coca::IAttribute& attribute )
{
    if ( !_node ) { return; }

    wxString name = _node->findAttributeName( &attribute );
    refreshValue( GetPropertyByName( name ), attribute );
}

void CocaPropertyGrid::refreshValue( wxPGProperty* property, const coca::IAttribute& attribute )
{
    if ( !_node || getAttribute( property ) != &attribute ) { return; }

    setValue( property, attribute.getAsString() );
    RefreshProperty( property );
}

void CocaPropertyGrid::updateValues()
{
    if ( GetChildrenCount() == 0 ) { return; }

    wxPGProperty* property = 0;
    for ( property = GetFirstVisible(); property != 0; property = GetNextVisible( property ) )
    {
        if ( property == GetSelectedProperty() /*&& property->IsTextEditable()*/ )
        {
            wxWindow* control = GetEditorControl();
            if ( control && control == wxWindow::FindFocus() ) { continue; }
        }

        const coca::IAttribute* attribute = getAttribute( property );
        if ( attribute ) { setValue( property, attribute->getAsString() ); }
    }
}

void CocaPropertyGrid::appendSink( const coca::IAttribute& attribute, const coca::IAttribute& sinkAttribute )
{
    wxPGProperty* property = findProperty( attribute );
    if ( !property ) { return; }

    COCA_DEBUG_INFO( "appendSink " << this << "; " << property->GetName() );

    wxPGProperty* sinks = property->GetPropertyByName( "sinks" );
    if ( !sinks ) { return; }

    addSink( sinkAttribute, sinks );
    refreshChildCount( sinks );
}

void CocaPropertyGrid::removeSink( const coca::IAttribute& attribute, const void* sinkAttribute )
{
    wxPGProperty* property = findProperty( attribute );
    if ( !property ) { return; }

    COCA_DEBUG_INFO( "removeSink " << this << "; " << property->GetName() );

    wxPGProperty* sinksProperty = property->GetPropertyByName( "sinks" );
    if ( !sinksProperty ) { return; }

    for ( size_t i = 0; i < sinksProperty->GetCount(); ++i )
    {
        if ( sinksProperty->Item( i )->GetClientData() == sinkAttribute )
        {
            DeleteProperty( sinksProperty->Item( i ) );
            refreshChildCount( sinksProperty, true );
            break;
        }
    }
}

void CocaPropertyGrid::appendSource( const coca::IAttribute& attribute, const coca::IAttribute& sourceAttribute )
{
    wxPGProperty* property = findProperty( attribute );
    if ( !property ) { return; }

    COCA_DEBUG_INFO( "appendSource " << this << "; " << property->GetName() );

    wxPGProperty* sources = property->GetPropertyByName( "sources" );
    if ( !sources )
    {
        refreshSource( attribute, property );
        return;
    }

    addSource( sourceAttribute, sources );
    refreshChildCount( sources );
}

void CocaPropertyGrid::removeSource( const coca::IAttribute& attribute, const void* sourceAttribute )
{
    wxPGProperty* property = findProperty( attribute );
    if ( !property ) { return; }

    COCA_DEBUG_INFO( "removeSource " << this << "; " << property->GetName() );

    wxPGProperty* sourcesProperty = property->GetPropertyByName( "sources" );
    if ( !sourcesProperty )
    {
        refreshSource( attribute, property );
        return;
    }

    for ( size_t i = 0; i < sourcesProperty->GetCount(); ++i )
    {
        if ( sourcesProperty->Item( i )->GetClientData() == sourceAttribute )
        {
            DeleteProperty( sourcesProperty->Item( i ) );
            refreshChildCount( sourcesProperty, true );
            break;
        }
    }
}

void CocaPropertyGrid::refreshSource( const coca::IAttribute& attribute )
{
    refreshSource( attribute, findProperty( attribute ) );
}

void CocaPropertyGrid::refreshSource( const coca::IAttribute& attribute, wxPGProperty* property )
{
    if ( !property ) { return; }
    COCA_DEBUG_INFO( "refreshSource " << this << "; " << property->GetName() );

    wxPGProperty* sourceProperty = property->GetPropertyByName( "source" );
    if ( !sourceProperty ) { return; }

    coca::IAttribute* source = 0;
    if ( attribute.getSources()->size() == 1 ) { source = attribute.getSources()->front(); }

    sourceProperty->SetClientData( source );

    if ( source ) { sourceProperty->SetValueFromString( EditorTools::getName( *source ) ); }
    else { sourceProperty->SetValueFromString( "" ); }

    RefreshProperty( sourceProperty );
}

void CocaPropertyGrid::refreshAttributeNames( const coca::INode& owner )
{
    wxPGProperty* property = 0;
    for ( property = GetFirstProperty(); property != 0; property = GetNextSiblingProperty( property ) )
    {
        refreshAttributeNames( owner, property );
    }
}

void CocaPropertyGrid::refreshAttributeNames( const coca::INode& owner, wxPGProperty* attributeProperty )
{
    if ( !isAttributeProperty( attributeProperty ) ) { return; }

    // sinks
    wxPGProperty* sinksProperty = attributeProperty->GetPropertyByName( "sinks" );

    if ( sinksProperty )
    {
        for ( size_t i = 0; i < sinksProperty->GetCount(); ++i )
        {
            refreshAttributeName( owner, sinksProperty->Item( i ) );
        }
    }

    // sources
    wxPGProperty* sourcesProperty = attributeProperty->GetPropertyByName( "sources" );

    if ( sourcesProperty )
    {
        for ( size_t i = 0; i < sourcesProperty->GetCount(); ++i )
        {
            refreshAttributeName( owner, sourcesProperty->Item( i ) );
        }
    }
    else
    {
        wxPGProperty* sourceProperty = attributeProperty->GetPropertyByName( "source" );
        if ( sourceProperty && sourceProperty->GetClientData() )
        {
            refreshAttributeName( owner, sourceProperty );
        }
    }
}

wxPGProperty* CocaPropertyGrid::getProperty( wxPoint point ) const
{
    wxPropertyGridHitTestResult hitResult = HitTest( point );
    return hitResult.GetProperty();
}

wxPGProperty* CocaPropertyGrid::findProperty( const coca::IAttribute& attribute )
{
    wxPGProperty* property = 0;
    for ( property = GetFirstProperty(); property != 0; property = GetNextSiblingProperty( property ) )
    {
        if ( getAttribute( property ) == &attribute ) { return property; }
    }
    return 0;
}

bool CocaPropertyGrid::isAttributeProperty( wxPGProperty* property ) const
{
    return ( property && property->GetParent() == GetRoot() );
}

bool CocaPropertyGrid::isSinksProperty( wxPGProperty* property ) const
{
    return ( property && isAttributeProperty( property->GetParent() )
             && property->GetBaseName() == "sinks" );
}

bool CocaPropertyGrid::isSinkProperty( wxPGProperty* property ) const
{
    return ( property && isSinksProperty( property->GetParent() ) );
}

bool CocaPropertyGrid::isSourcesProperty( wxPGProperty* property ) const
{
    return ( property && isAttributeProperty( property->GetParent() )
             && property->GetBaseName() == "sources" );
}

bool CocaPropertyGrid::isSourceProperty( wxPGProperty* property ) const
{
    return ( property &&
             ( isSourcesProperty( property->GetParent() ) ||
               ( isAttributeProperty( property->GetParent() ) && property->GetBaseName() == "source" ) ) );
}

const coca::IAttribute* CocaPropertyGrid::getAttribute( wxPGProperty* property ) const
{
    if ( !isAttributeProperty( property ) ) { return 0; }
    return reinterpret_cast<coca::IAttribute*>( property->GetClientData() );
}

const coca::IAttribute::AttributeVector* CocaPropertyGrid::getSinks( wxPGProperty* property ) const
{
    if ( !isSinksProperty( property ) ) { return 0; }
    return reinterpret_cast<coca::IAttribute::AttributeVector*>( property->GetClientData() );
}

const coca::IAttribute* CocaPropertyGrid::getSink( wxPGProperty* property ) const
{
    if ( !isSinkProperty( property ) ) { return 0; }
    return reinterpret_cast<coca::IAttribute*>( property->GetClientData() );
}

const coca::IAttribute::AttributeVector* CocaPropertyGrid::getSources( wxPGProperty* property ) const
{
    if ( !isSourcesProperty( property ) ) { return 0; }
    return reinterpret_cast<coca::IAttribute::AttributeVector*>( property->GetClientData() );
}

const coca::IAttribute* CocaPropertyGrid::getSource( wxPGProperty* property ) const
{
    if ( !isSourceProperty( property ) ) { return 0; }
    if ( property->GetClientData() )
    {
        return reinterpret_cast<coca::IAttribute*>( property->GetClientData() );
    }
    else
    {
        const coca::IAttribute* attribute = getAttribute( property->GetParent() );
        COCA_ASSERT( attribute );
        COCA_ASSERT( attribute->getSources() );
        if ( attribute->getSources()->size() == 1 )
        {
            return reinterpret_cast<coca::IAttribute*>( attribute->getSources()->front() );
        }
        return 0;
    }
}

const coca::IAttribute* CocaPropertyGrid::findAttribute( wxPGProperty* property ) const
{
    while ( property && property->GetParent() != GetRoot() )
    {
        property = property->GetParent();
    }
    return getAttribute( property );
}

std::string CocaPropertyGrid::getValue( wxPGProperty* property ) const
{
    wxString value;
    if ( !property )
    {
    }
    else if ( property->GetType() == "bool" )
    {
        value << property->GetValue().GetBool();
    }
    else
    {
        value = property->GetValueString();
    }
    return value.c_str();
}

bool CocaPropertyGrid::setValue( wxPGProperty* property, const std::string& value ) const
{
    if ( !property )
    {
        return false;
    }
    else if ( property->GetType() == "bool" )
    {
        return property->SetValueFromInt( value != "0" );
    }
    else
    {
        return property->SetValueFromString( value );
    }
}

// states //

const CocaPropertyGrid::StateMap& CocaPropertyGrid::getStates() const
{
    return _states;
}

wxString CocaPropertyGrid::findState( const coca::INode* node ) const
{
    StateMap::const_iterator it = _states.find( node );
    return ( it != _states.end() ? it->second : "" );
}

void CocaPropertyGrid::storeState( const coca::INode* node, const wxString& state )
{
    if ( node ) { _states[node] = state; }
}

// private //

void CocaPropertyGrid::add( const coca::IAttribute& attribute, const std::string& name )
{
    wxPGProperty* property = 0;
    wxPGProperty* attributeProperty = Append( createProperty( attribute, name ) );

    if ( attribute.isMultipleInputAttribute() )
    {
        wxPGProperty* sourcesProperty = AppendIn( attributeProperty, new wxStringProperty( "sources" ) );
        addSources( attribute, sourcesProperty );
    }
    else if ( attribute.isInputAttribute() )
    {
        property = AppendIn( attributeProperty, new wxStringProperty( "source" ) );
        if ( !attribute.getSources()->empty() )
        {
            const coca::IAttribute* source = attribute.getSources()->front();
            property->SetValueFromString( EditorTools::getName( *source ) );
            property->SetClientData( const_cast<coca::IAttribute*>( source ) );
        }
        setEditable( property, false );
    }

    if ( attribute.isOutputAttribute() )
    {
        wxPGProperty* sinksProperty = AppendIn( attributeProperty, new wxStringProperty( "sinks" ) );
        addSinks( attribute, sinksProperty );
    }
    Collapse( attributeProperty );
}

void CocaPropertyGrid::addSinks( const coca::IAttribute& attribute, wxPGProperty* sinksProperty )
{
    const coca::IAttribute::AttributeVector* sinks = attribute.getSinks();
    sinksProperty->SetClientData( const_cast<coca::IAttribute::AttributeVector*>( sinks ) );
    setEditable( sinksProperty, false );
    coca::IAttribute::AttributeVector::const_iterator it;
    for ( it = sinks->begin(); it != sinks->end(); ++it )
    {
        addSink( **it, sinksProperty );
    }
    refreshChildCount( sinksProperty );
}

void CocaPropertyGrid::addSink( const coca::IAttribute& attribute, wxPGProperty* sinksProperty )
{
    wxPGProperty* property = AppendIn( sinksProperty,
                                       new wxStringProperty( wxString() << sinksProperty->GetChildCount() ) );
    //property->SetValueFromString( getPointerString( &attribute ) );
    property->SetValueFromString( EditorTools::getName( attribute ) );
    property->SetClientData( const_cast<coca::IAttribute*>( &attribute ) );
    setEditable( property, false );
}

void CocaPropertyGrid::addSources( const coca::IAttribute& attribute, wxPGProperty* sourcesProperty )
{
    const coca::IAttribute::AttributeVector* sources = attribute.getSources();
    sourcesProperty->SetClientData( const_cast<coca::IAttribute::AttributeVector*>( sources ) );
    setEditable( sourcesProperty, false );
    coca::IAttribute::AttributeVector::const_iterator it;
    for ( it = sources->begin(); it != sources->end(); ++it )
    {
        addSource( **it, sourcesProperty );
    }
    refreshChildCount( sourcesProperty );
}

void CocaPropertyGrid::addSource( const coca::IAttribute& attribute, wxPGProperty* sourcesProperty )
{
    wxPGProperty* property = AppendIn( sourcesProperty,
                                       new wxStringProperty( wxString() << sourcesProperty->GetChildCount() ) );
    //property->SetValueFromString( getPointerString( &attribute ) );
    property->SetValueFromString( EditorTools::getName( attribute ) );
    property->SetClientData( const_cast<coca::IAttribute*>( &attribute ) );
    setEditable( property, false );
}

void CocaPropertyGrid::refreshChildCount( wxPGProperty* property, bool refreshChildIndices )
{
    property->SetValueFromString( wxString( "(" ) << property->GetChildCount() << ")" );

    if ( refreshChildIndices )
    {
        for ( size_t i = 0; i < property->GetCount(); ++i )
        {
            property->Item( i )->SetLabel( wxString() << i );
        }
    }
}

wxPGProperty* CocaPropertyGrid::createProperty( const coca::IAttribute& attribute, const std::string& name )
{
    const std::type_info& typeInfo = attribute.getTypeInfo();

    wxPGProperty* property = 0;
    if ( typeInfo == typeid( coca::bool8 ) )
    {
        /// @todo: report: wxBoolProperty doesn't handly events very well when not editable
        if ( attribute.canBeSetAsString() )
        {
            property = new wxBoolProperty( name, name );
            wxString str( wxPG_BOOL_USE_CHECKBOX );
            wxVariant var( true );
            property->SetAttribute( str, var );
        }
        else
        {
            property = new wxUIntProperty( name, name );
            wxString str( "Max" );
            wxVariant var( 1 );
            property->SetAttribute( str, var );
        }
    }
    else if ( typeInfo == typeid( coca::int8 ) || typeInfo == typeid( coca::int16 ) ||
              typeInfo == typeid( coca::int32 ) || typeInfo == typeid( coca::int64 ) )
    {
        property = new wxIntProperty( name, name );
    }
    else if ( typeInfo == typeid( coca::uint8 ) || typeInfo == typeid( coca::uint16 ) ||
              typeInfo == typeid( coca::uint32 ) || typeInfo == typeid( coca::uint64 ) )
    {
        property = new wxUIntProperty( name, name );
    }
    else if ( typeInfo == typeid( coca::float32 ) || typeInfo == typeid( coca::float64 ) )
    {
        property = new wxFloatProperty( name, name );
    }
    else if ( typeInfo == typeid( void ) )
    {
        property = new wxStringProperty( name, name );
        setEditable( property, false );
    }
    else
    {
        property = new wxLongStringProperty( name, name );
    }

    setEditable( property, attribute.canBeSetAsString() );
    setValue( property, attribute.getAsString() );
    SetPropertyCell( property, E_INFO_COLUMN, wxString( "(" ) + attribute.getTypeInfo().name() + ")" );
    property->SetClientData( const_cast<coca::IAttribute*>( &attribute ) );

    return property;
}

void CocaPropertyGrid::setEditable( wxPGProperty* property, bool editable )
{
    if ( !property ) { return; }
    if ( editable ) { property->ClearFlag( wxPG_PROP_READONLY ); }
    else { property->SetFlag( wxPG_PROP_READONLY ); }
}

wxString CocaPropertyGrid::getPointerString( const void* pointer )
{
    std::ostringstream ss;
    ss << pointer;
    return ss.str();
}

void CocaPropertyGrid::refreshAttributeName( const coca::INode& owner, wxPGProperty* attributeProperty )
{
    const coca::IAttribute* attribute = reinterpret_cast<coca::IAttribute*>( attributeProperty->GetClientData() );
    COCA_ASSERT( attribute );
    if ( attribute->getOwner() == &owner )
    {
        attributeProperty->SetValueFromString( EditorTools::getName( *attribute ) );
        RefreshProperty( attributeProperty );
    }
}

