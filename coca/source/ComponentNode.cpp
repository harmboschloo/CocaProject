// Copyright (C) 2008-2009 Harm Boschloo

#include "ComponentNode.h"
#include <coca/IComponent.h>
#include <coca/IFactory.h>
#include <coca/IEventManager.h>
#include <coca/attributes/IAttribute.h>
#include <coca/log/log.h>

namespace coca
{
    ComponentNode::ComponentNode( const std::string& componentId, const std::string& name, INode* parent ) :
            ANode( name, parent ),
            _component( 0 )
    {
        resetComponent( componentId );
    }

    ComponentNode::~ComponentNode()
    {
        COCA_DEBUG_INFO( "~ComponentNode " << getName() << ":" << getComponentId() );
        resetComponent();
        COCA_DEBUG_INFO( "~ComponentNode end " << getName() << ":" << getComponentId() );
    }

    // Attributes //

    bool ComponentNode::addAttribute( const std::string& name, IAttribute* attribute )
    {
        if ( !attribute ) { return false; }

        AttributeMap::iterator it = _attributes.find( name );
        if ( it != _attributes.end() )
        {
            COCA_WARNING( "Overwriting attribute " << name << " of node " << getName() );
            doRemoveAttribute( it );
        }

        _attributes[name] = attribute;
        setOwnerOf( attribute );
        setParentEnabled( attribute, isEnabled() );

        getEventManager().sendEvent( E_NODE_ATTRIBUTE_ADDED_EVENT, *this, *attribute );

        return true;
    }

    // Component //

    bool ComponentNode::isComponentNode() const
    {
        return true;
    }

    bool ComponentNode::resetComponent( const std::string& componentId )
    {
        bool wasEnabled = isEnabled();
        if ( wasEnabled ) { disable(); }

        // remove old component
        clearAttributes();
        delete _component;
        _component = 0;
        _componentId = componentId;

        // set new component
        if ( !_componentId.empty() )
        {
            _component = getFactory().createComponent( _componentId );
            if ( !_component || !_component->init( *this ) )
            {
                if ( _component )
                {
                    COCA_ERROR( "Could not initialize component with id '" << _componentId << "'" );
                    delete _component;
                    _component = 0;
                }
                _componentId = "";
            }
        }

        if ( wasEnabled ) { enable(); }

        getEventManager().sendEvent( E_NODE_COMPONENT_CHANGED_EVENT, *this );

        return ( _componentId == componentId );
    }

    const std::string& ComponentNode::getComponentId() const
    {
        return _componentId;
    }

    const IComponent* ComponentNode::getComponent() const
    {
        return _component;
    }

    IComponent* ComponentNode::getComponent()
    {
        return _component;
    }

    // private //

    void ComponentNode::onEnabled()
    {
        if ( _component ) { _component->onEnabled(); }
        setParentEnabledForAttributes( true );
    }

    void ComponentNode::onDisabled()
    {
        if ( _component ) { _component->onDisabled(); }
        setParentEnabledForAttributes( false );
    }

    void ComponentNode::doRemoveAttribute( AttributeMap::iterator it )
    {
        IAttribute* attribute = it->second;
        _attributes.erase( it );
        getEventManager().sendEvent( E_NODE_ATTRIBUTE_REMOVED_EVENT, *this, *attribute );
        destroy( attribute );
    }

    void ComponentNode::setParentEnabledForAttributes( bool enabled )
    {
        AttributeMap::const_iterator it;
        for ( it = _attributes.begin(); it != _attributes.end(); ++it )
        {
            setParentEnabled( it->second, enabled );
        }
    }

} // namespace coca
