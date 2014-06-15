// Copyright (C) 2007-2008 Harm Boschloo

#include "AttributeConnectionMenu.h"
#include "EditorTools.h"
#include <coca/attributes/IAttribute.h>
#include <coca/log/log.h>

BEGIN_EVENT_TABLE( AttributeConnectionMenu, wxMenu )
    EVT_MENU( wxID_ANY, AttributeConnectionMenu::onMenu )
END_EVENT_TABLE()

AttributeConnectionMenu::AttributeConnectionMenu( const coca::INode& sourceNode, const coca::IAttribute& sourceAttribute,
        const coca::INode& sinkNode )
{
    reset( sourceNode, sourceAttribute, sinkNode );
}

AttributeConnectionMenu::AttributeConnectionMenu( const coca::INode& sourceNode,
        const coca::INode& sinkNode, const coca::IAttribute& sinkAttribute )
{
    reset( sourceNode, sinkNode, sinkAttribute );
}

AttributeConnectionMenu::~AttributeConnectionMenu()
{

}

void AttributeConnectionMenu::onMenu( wxCommandEvent& event )
{
    int index = toIndex( event.GetId() );

    if ( index >= 0 && index < static_cast<int>( _connections.size() ) )
    {
        _selectedConnection = index;
    }
}

bool AttributeConnectionMenu::hasConnections() const
{
    return !_connections.empty();
}

const coca::IAttribute* AttributeConnectionMenu::getSelectedSource() const
{
    if ( _selectedConnection < 0 ) { return 0; }
    COCA_ASSERT( _selectedConnection < static_cast<int>( _connections.size() ) );
    return _connections[_selectedConnection].first;
}

const coca::IAttribute* AttributeConnectionMenu::getSelectedSink() const
{
    if ( _selectedConnection < 0 ) { return 0; }
    COCA_ASSERT( _selectedConnection < static_cast<int>( _connections.size() ) );
    return _connections[_selectedConnection].second;
}

void AttributeConnectionMenu::reset()
{
    SetTitle( "connect attribute" );
    _selectedConnection = -1;
    _connections.clear();
}

void AttributeConnectionMenu::reset( const coca::INode& sourceNode, const coca::IAttribute& sourceAttribute,
                                     const coca::INode& sinkNode )
{
    reset();

    if ( !sourceAttribute.isOutputAttribute() ) { return; }

    wxString sourceAttributeName( EditorTools::getName( sourceAttribute ) );

    const coca::INode::AttributeMap& sinkAttributes = sinkNode.getAttributes();
    coca::INode::AttributeMap::const_iterator it;
    for ( it = sinkAttributes.begin(); it != sinkAttributes.end(); ++it )
    {
        bool enable = ( it->second->isInputAttribute()
                        && it->second->getTypeInfo() == sourceAttribute.getTypeInfo() );

        wxString sinkAttributeName( EditorTools::getName( *it->second ) );
        wxMenuItem* item = Append( getNextId(), sourceAttributeName + " -> " + sinkAttributeName );
        item->Enable( enable );
        _connections.push_back( std::make_pair( &sourceAttribute, it->second ) );
    }
}

void AttributeConnectionMenu::reset( const coca::INode& sourceNode,
                                     const coca::INode& sinkNode, const coca::IAttribute& sinkAttribute )
{
    reset();

    if ( !sinkAttribute.isInputAttribute() ) { return; }

    wxString sinkAttributeName( EditorTools::getName( sinkAttribute ) );

    const coca::INode::AttributeMap& sourceAttributes = sourceNode.getAttributes();
    coca::INode::AttributeMap::const_iterator it;
    for ( it = sourceAttributes.begin(); it != sourceAttributes.end(); ++it )
    {
        bool enable = ( it->second->isOutputAttribute()
                        && it->second->getTypeInfo() == sinkAttribute.getTypeInfo() );
        wxString sourceAttributeName( EditorTools::getName( *it->second ) );
        wxMenuItem* item = Append( getNextId(), ( sourceAttributeName + " -> " + sinkAttributeName ) );
        item->Enable( enable );
        _connections.push_back( std::make_pair( it->second, &sinkAttribute ) );
    }
}

int AttributeConnectionMenu::getNextId() const
{
    return E_ID_LAST+_connections.size();
}

int AttributeConnectionMenu::toIndex( int id ) const
{
    return id - E_ID_LAST;
}
