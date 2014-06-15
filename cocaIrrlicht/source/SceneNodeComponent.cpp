// Copyright (C) 2007-2008 Harm Boschloo

#include "SceneNodeComponent.h"
#include "irrlicht_io.h"
#include <coca/INode.h>
#include <coca/attributes.h>

SceneNodeComponent::SceneNodeComponent( bool createNodeOutput ) :
        _createNodeOutput( createNodeOutput ),
        _parentAccessor( 0 ),
        _visibleAccessor( 0 ),
        _positionAccessor( 0 ),
        _rotationAccessor( 0 ),
        _scaleAccessor( 0 ),
        _node( 0 ),
        _nodeAttribute( 0 )
{
    _parentAccessor = coca::makeConditionalAccessor<irr::scene::ISceneNode,irr::scene::ISceneNode*>(
                          ( irr::scene::ISceneNode* )0, &irr::scene::ISceneNode::getParent,
                          &irr::scene::ISceneNode::setParent );
    _parentAccessor( 0 ); // default value
    _visibleAccessor = coca::makeConditionalAccessor<irr::scene::ISceneNode,bool>(
                           ( irr::scene::ISceneNode* )0, &irr::scene::ISceneNode::isVisible,
                           &irr::scene::ISceneNode::setVisible );
    _visibleAccessor( true ); // default value
    _positionAccessor = coca::makeConditionalAccessor<irr::scene::ISceneNode,irr::core::vector3df>(
                            ( irr::scene::ISceneNode* )0, &irr::scene::ISceneNode::getPosition,
                            &irr::scene::ISceneNode::setPosition );
    _rotationAccessor = coca::makeConditionalAccessor<irr::scene::ISceneNode,irr::core::vector3df>(
                            ( irr::scene::ISceneNode* )0, &irr::scene::ISceneNode::getRotation,
                            &irr::scene::ISceneNode::setRotation );
    _scaleAccessor = coca::makeConditionalAccessor<irr::scene::ISceneNode,irr::core::vector3df>(
                         ( irr::scene::ISceneNode* )0, &irr::scene::ISceneNode::getScale,
                         &irr::scene::ISceneNode::setScale );
    _scaleAccessor( irr::core::vector3df( 1, 1, 1 ) ); // default value

}

SceneNodeComponent::~SceneNodeComponent()
{
    COCA_DEBUG_INFO( "~SceneNodeComponent" );
}

bool SceneNodeComponent::init( coca::INode& node )
{
    coca::IAttribute* attribute = 0;

    attribute = coca::createInputOutputAttribute<irr::scene::ISceneNode*>( _parentAccessor );
    attribute->appendUpdateCallback( coca::makeFunction( *this, &SceneNodeComponent::onParentChanged ) );
    node.addAttribute( "parent", attribute );

    attribute = coca::createInputOutputAttribute<bool>( _visibleAccessor );
    node.addAttribute( "visible", attribute );

    attribute = coca::createInputOutputAttribute<irr::core::vector3df>( _positionAccessor );
    node.addAttribute( "position", attribute );

    attribute = coca::createInputOutputAttribute<irr::core::vector3df>( _rotationAccessor );
    node.addAttribute( "rotation", attribute );

    attribute = coca::createInputOutputAttribute<irr::core::vector3df>( _scaleAccessor );
    node.addAttribute( "scale", attribute );

    if ( _createNodeOutput )
    {
        _nodeAttribute = coca::createOutputAttribute<irr::scene::ISceneNode*>( coca::makeAccessor( _node ) );
        node.addAttribute( "node", _nodeAttribute );
    }

    return true;
}

void SceneNodeComponent::onEnabled()
{
    COCA_ASSERT_EQUAL( _node, ( void* )0 );
    if ( _parentAccessor() ) { createNode(); }
}

void SceneNodeComponent::onDisabled()
{
    if ( _node ) { dropNode(); }
}

bool SceneNodeComponent::createAndGrabNode( irr::scene::ISceneManager& manager )
{
    _node = manager.addEmptySceneNode( _parentAccessor() );
    _node->grab();
    return true;
}

void SceneNodeComponent::onParentChanged()
{
    COCA_DEBUG_INFO( "SceneNodeComponent::onParentChanged() " << _node << "; " << _parentAccessor() );
    if ( _parentAccessor() == 0 )
    {
        if ( _node ) { dropNode(); }
    }
    else if ( !_node )
    {
        createNode();
    }
    // else parent is set by accessor

    COCA_DEBUG_INFO( "SceneNodeComponent::onParentChanged() end " << _node << "; " << _parentAccessor() );
}

void SceneNodeComponent::createNode()
{
    COCA_DEBUG_INFO( "SceneNodeComponent::createNode() " << _node << "; " << _parentAccessor() );
    COCA_ASSERT_EQUAL( _node, ( void* )0 );
    COCA_ASSERT( _parentAccessor() );
    COCA_ASSERT( _parentAccessor()->getSceneManager() );

    if ( createAndGrabNode( *( _parentAccessor()->getSceneManager() ) ) )
    {
        setAccessorObject( _node );
        onNodeCreated();
        updateNodeOutput();
    }
}

void SceneNodeComponent::dropNode()
{
    COCA_ASSERT( _node );

    setAccessorObject( 0 );
    _node->remove();
    _node->drop();
    _node = 0;
    onNodeRemoved();
    updateNodeOutput();
}

void SceneNodeComponent::setAccessorObject( irr::scene::ISceneNode* node )
{
    _parentAccessor.setObject( node );
    _visibleAccessor.setObject( node );
    _positionAccessor.setObject( node );
    _rotationAccessor.setObject( node );
    _scaleAccessor.setObject( node );
}

void SceneNodeComponent::updateNodeOutput()
{
    if ( _nodeAttribute ) { _nodeAttribute->update(); }
}
