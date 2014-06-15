// Copyright (C) 2008-2009 Harm Boschloo

#include "ANode.h"
#include <coca/IEventManager.h>
#include <coca/attributes/IAttribute.h>
#include <coca/utils/algorithm.h>
#include <coca/utils/function.h>
#include <coca/utils/string.h>
#include <coca/log/log.h>
#include <coca/interface.h>

namespace coca
{
    // static
    const INode::NodeVector ANode::EMPTY_NODE_VECTOR;

    ANode::ANode( const std::string& name, INode* parent ) :
            _disabledCount( 0 ),
            _name( name ),
            _parent( 0 )
    {
        if ( parent ) { parent->append( this ); }
    }

    ANode::~ANode()
    {
        COCA_DEBUG_INFO( "~ANode " << getName() );
        // can not clear attributes here because then a virtual method of a child class is called!
    }

    // Name //

    const std::string& ANode::getName() const
    {
        return _name;
    }

    void ANode::setName( const std::string& name )
    {
        if ( name == _name ) { return; }

        _name = name;
        getEventManager().sendEvent( E_NODE_NAME_CHANGED_EVENT, *this );
    }

    // Parent and root //
    const INode* ANode::getParent() const
    {
        return _parent;
    }

    INode* ANode::getParent()
    {
        return _parent;
    }

    bool ANode::isRoot() const
    {
        return ( _parent == 0 );
    }

    const INode& ANode::getRoot() const
    {
        if ( _parent ) { return _parent->getRoot(); }
        else { return *this; }
    }

    INode& ANode::getRoot()
    {
        if ( _parent ) { return _parent->getRoot(); }
        else { return *this; }
    }

    // State //

    bool ANode::isEnabled() const
    {
        return ( _disabledCount == 0 );
    }

    bool ANode::enable( bool enableChildren )
    {
        if ( _parent && _parent->getDisabledCount() >= _disabledCount )
        {
            COCA_DEBUG_WARNING( "Can not enable node due to parent's disabled count" );
            return isEnabled();
        }

        if ( _disabledCount > 0 )
        {
            --_disabledCount;
            if ( _disabledCount == 0 ) { onEnabled(); }

            getEventManager().sendEvent( E_NODE_DISABLED_COUNT_CHANGED_EVENT, *this );
        }

        if ( enableChildren )
        {
            forAll( getChildren(), makePtrMemberFunction( &INode::enable ), enableChildren );
        }

        return isEnabled();
    }

    bool ANode::isDisabled() const
    {
        return !isEnabled();
    }

    void ANode::disable( bool disableChildren )
    {
        ++_disabledCount;
        if ( _disabledCount == 1 ) { onDisabled(); }

        getEventManager().sendEvent( E_NODE_DISABLED_COUNT_CHANGED_EVENT, *this );

        if ( disableChildren )
        {
            forAll( getChildren(), makePtrMemberFunction( &INode::disable ), disableChildren );
        }
        else
        {
            // check children disabled count
            // can not be less than our disabled count
            const NodeVector& children = getChildren();
            NodeVector::const_iterator it;
            for ( it = children.begin(); it != children.end(); ++it )
            {
                if (( *it )->getDisabledCount() < _disabledCount )
                {
                    ( *it )->disable( false );
                }
            }
        }
    }

    int ANode::getDisabledCount() const
    {
        return _disabledCount;
    }

    void ANode::setDisabledCount( int disabledCount )
    {
        _disabledCount = disabledCount;
    }

    // Attributes //

    bool ANode::removeAttribute( IAttribute* attribute )
    {
        AttributeMap::iterator it;
        for ( it = _attributes.begin(); it != _attributes.end(); ++it )
        {
            if ( it->second == attribute )
            {
                //removeAttributeFromParent( attribute );
                doRemoveAttribute( it );
                return true;
            }
        }
        return false;
    }

    bool ANode::removeAttribute( const std::string& name )
    {
        AttributeMap::iterator it = _attributes.find( name );
        if ( it != _attributes.end() )
        {
            //removeAttributeFromParent( it->second );
            doRemoveAttribute( it );
            return true;
        }
        return false;
    }

    void ANode::clearAttributes()
    {
        AttributeMap::iterator it;
        AttributeMap::iterator it2;
        for ( it = _attributes.begin(); it != _attributes.end(); )
        {
            it2 = it;
            ++it;
            //removeAttributeFromParent( it->second );
            doRemoveAttribute( it2 );
        }
    }

    bool ANode::hasAttribute( const IAttribute* attribute ) const
    {
        AttributeMap::const_iterator it;
        for ( it = _attributes.begin(); it != _attributes.end(); ++it )
        {
            if ( it->second == attribute ) { return true; }
        }
        return false;
    }

    const std::string& ANode::findAttributeName( const IAttribute* attribute ) const
    {
        AttributeMap::const_iterator it;
        for ( it = _attributes.begin(); it != _attributes.end(); ++it )
        {
            if ( it->second == attribute ) { return it->first; }
        }
        return EMPTY_STRING;
    }

    const IAttribute* ANode::findAttribute( const std::string& name ) const
    {
        AttributeMap::const_iterator it = _attributes.find( name );
        return ( it != _attributes.end() ? it->second : 0 );
    }

    IAttribute* ANode::findAttribute( const std::string& name )
    {
        AttributeMap::iterator it = _attributes.find( name );
        return ( it != _attributes.end() ? it->second : 0 );
    }

    const ANode::AttributeMap& ANode::getAttributes() const
    {
        return _attributes;
    }

    void ANode::setAttributesEnabled( bool enabled ) const
    {
        AttributeMap::const_iterator it;
        for ( it = _attributes.begin(); it != _attributes.end(); ++it )
        {
            it->second->setEnabled( enabled );
        }
    }

    // Component //

    bool ANode::isComponentNode() const
    {
        return false;
    }

    bool ANode::resetComponent( const std::string& )
    {
        return false;
    }

    const std::string& ANode::getComponentId() const
    {
        return EMPTY_STRING;
    }

    const IComponent* ANode::getComponent() const
    {
        return 0;
    }

    IComponent* ANode::getComponent()
    {
        return 0;
    }

    // Composite //

    bool ANode::isCompositeNode() const
    {
        return false;
    }

    const INode::NodeVector& ANode::getChildren() const
    {
        return EMPTY_NODE_VECTOR;
    }

    bool ANode::append( INode* child )
    {
        COCA_ERROR( "Can not append child, '" << _name << "' is not a composite node" );
        NodePtr ptr( child );
        return false;
    }

    bool ANode::remove( INode* )
    {
        return false;
    }

    void ANode::clearChildren()
    {
    }

    bool ANode::moveUp( INode* )
    {
        return false;
    }

    bool ANode::moveDown( INode* )
    {
        return false;
    }

    // protected //

    void ANode::setParent( INode* parent )
    {
        _parent = parent;
    }

    void ANode::setOwnerOf( IAttribute* attribute )
    {
        COCA_ASSERT( attribute );
        attribute->setOwner( this );
    }

    void ANode::destroy( IAttribute* attribute )
    {
        COCA_ASSERT( attribute );
        COCA_ASSERT_EQUAL( attribute->getOwner(), this );
        delete attribute;
    }

    /*void ANode::removeAttributeFromParent( IAttribute* attribute )
    {
        if ( _parent ) { _parent->removeAttribute( attribute ); }
    }*/

    void ANode::setParentEnabled( IAttribute* attribute, bool enabled )
    {
        COCA_ASSERT( attribute );
        attribute->setParentEnabled( enabled );
    }

    void ANode::onEnabled()
    {
    }

    void ANode::onDisabled()
    {
    }

} // namespace coca
