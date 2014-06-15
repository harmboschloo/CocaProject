// Copyright (C) 2008-2009 Harm Boschloo

#include "CompositeNode.h"
#include <coca/IEventManager.h>
#include <coca/interface.h>
#include <coca/log/log.h>
#include <coca/utils/algorithm.h>

namespace coca
{
    CompositeNode::CompositeNode( const std::string& name, INode* parent ) :
            ANode( name, parent )
    {
    }

    CompositeNode::~CompositeNode()
    {
        clearChildren();
    }

    // Attributes //

    /// @todo: use a wrapper attribute instead
    bool CompositeNode::addAttribute( const std::string&, IAttribute* )
    {
        COCA_ASSERT( false );
        return false;

        /*if ( !attribute ) { return false; }

        // can only add attributes from child nodes
        bool found = false;
        const NodeVector& children = getChildren();
        NodeVector::const_iterator it;
        for ( it = children.begin(); it != children.end(); ++it )
        {
            if (( *it )->hasAttribute( attribute ) )
            {
                found = true;
                break;
            }
        }

        if ( !found )
        {
            COCA_ERROR( "Can only add an attribute to a composite node if it is from a child node!" );
            return false;
        }

        AttributeMap::iterator attributeIt = _attributes.find( name );
        if ( attributeIt != _attributes.end() )
        {
            COCA_WARNING( "Overwriting attribute " << name << " of module node " << getName() );
            removeAttributeFromParent( attributeIt->second );
            doRemoveAttribute( attributeIt );
        }

        _attributes[name] = attribute;

        getEventManager().sendEvent( E_NODE_ATTRIBUTE_ADDED_EVENT, *this, *attribute );

        return true;*/
    }

    // Composite //

    bool CompositeNode::isCompositeNode() const
    {
        return true;
    }

    const INode::NodeVector& CompositeNode::getChildren() const
    {
        return _children;
    }

    bool CompositeNode::append( INode* child )
    {
        NodePtr ptr( child ); // scoped reference count

        if ( !child || child == this || child->getParent() == this ) { return false; }

        _children.push_back( ptr );
        if ( child->getParent() ) { child->getParent()->remove( child ); }
        child->setParent( this );

        // check disabled count
        while ( child->getDisabledCount() < getDisabledCount() )
        {
            child->disable();
        }

        getEventManager().sendEvent( E_NODE_CHILD_APPENDED_EVENT, *this, *child );

        return true;
    }

    bool CompositeNode::remove( INode* child )
    {
        NodePtr ptr( child ); // scoped reference count

        if ( !child || child->getParent() != this ) { return false; }
        if ( !eraseItem( _children, ptr ) ) { return false; }

        child->setParent( 0 );

        // check for added child attributes
        /*AttributeMap::iterator it;
        AttributeMap::iterator it2;
        for ( it = _attributes.begin(); it != _attributes.end(); )
        {
            it2 = it;
            ++it;

            if ( child->hasAttribute( it2->second ) )
            {
                removeAttributeFromParent( it2->second );
                doRemoveAttribute( it2 );
            }
        }*/

        getEventManager().sendEvent( E_NODE_CHILD_REMOVED_EVENT, *this, *child );

        return true;
    }

    void CompositeNode::clearChildren()
    {
        // clear added attributes from children
        clearAttributes();

        // clear children
        NodeVector::iterator it;
        for ( it = _children.begin(); it != _children.end(); )
        {
            NodePtr child( *it ); // scoped reference count
            child->setParent( 0 );
            it = _children.erase( it );
            getEventManager().sendEvent( E_NODE_CHILD_REMOVED_EVENT, *this, *child );
        }
    }

    bool CompositeNode::moveUp( INode* child )
    {
        if ( coca::moveUp( _children, child ) )
        {
            getEventManager().sendEvent( E_NODE_CHILD_MOVED_UP_EVENT, *this, *child );
            return true;
        }
        return false;
    }

    bool CompositeNode::moveDown( INode* child )
    {
        if ( coca::moveDown( _children, child ) )
        {
            getEventManager().sendEvent( E_NODE_CHILD_MOVED_DOWN_EVENT, *this, *child );
            return true;
        }
        return false;
    }

    // private //

    void CompositeNode::doRemoveAttribute( AttributeMap::iterator it )
    {
        /*IAttribute* attribute = it->second;
        _attributes.erase( it );
        getEventManager().sendEvent( E_NODE_ATTRIBUTE_REMOVED_EVENT, *this, *attribute );*/
    }

} // namespace coca
