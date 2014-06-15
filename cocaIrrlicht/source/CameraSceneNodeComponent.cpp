// Copyright (C) 2007-2008 Harm Boschloo

#include "CameraSceneNodeComponent.h"
#include "irrlicht_io.h"
#include <coca/INode.h>
#include <coca/attributes.h>

CameraSceneNodeComponent::CameraSceneNodeComponent() :
        _cameraNode( 0 ),
        _inputReceiverEnabledAccessor( 0 ),
        _bindTargetAndRotationAccessor( 0 ),
        _fovAccessor( 0 ),
        _aspectRationAccessor( 0 ),
        _nearValueAccessor( 0 ),
        _farValueAccessor( 0 ),
        _targetAccessor( 0 ),
        _upVectorAccessor( 0 ),
        _cameraNodeAttribute( 0 )
{
    _inputReceiverEnabledAccessor = coca::makeConditionalAccessor<irr::scene::ICameraSceneNode,bool>(
                                        ( irr::scene::ICameraSceneNode* )0,
                                        &irr::scene::ICameraSceneNode::isInputReceiverEnabled,
                                        &irr::scene::ICameraSceneNode::setInputReceiverEnabled );
    _inputReceiverEnabledAccessor( true ); // default value

    _bindTargetAndRotationAccessor = coca::makeConditionalAccessor<irr::scene::ICameraSceneNode,bool>(
                                        ( irr::scene::ICameraSceneNode* )0,
                                        &irr::scene::ICameraSceneNode::getTargetAndRotationBinding,
                                        &irr::scene::ICameraSceneNode::bindTargetAndRotation );
    _bindTargetAndRotationAccessor( true ); // default value

    _fovAccessor = coca::makeConditionalAccessor<irr::scene::ICameraSceneNode,irr::f32>(
                       ( irr::scene::ICameraSceneNode* )0,
                       &irr::scene::ICameraSceneNode::getFOV,
                       &irr::scene::ICameraSceneNode::setFOV );
    _fovAccessor( irr::core::PI/2.5f ); // default value

    _aspectRationAccessor = coca::makeConditionalAccessor<irr::scene::ICameraSceneNode,irr::f32>(
                       ( irr::scene::ICameraSceneNode* )0,
                       &irr::scene::ICameraSceneNode::getAspectRatio,
                       &irr::scene::ICameraSceneNode::setAspectRatio );
    _aspectRationAccessor( 4.f/3.f ); // default value

    _nearValueAccessor = coca::makeConditionalAccessor<irr::scene::ICameraSceneNode,irr::f32>(
                       ( irr::scene::ICameraSceneNode* )0,
                       &irr::scene::ICameraSceneNode::getNearValue,
                       &irr::scene::ICameraSceneNode::setNearValue );
    _nearValueAccessor( 1.f ); // default value

    _farValueAccessor = coca::makeConditionalAccessor<irr::scene::ICameraSceneNode,irr::f32>(
                       ( irr::scene::ICameraSceneNode* )0,
                       &irr::scene::ICameraSceneNode::getFarValue,
                       &irr::scene::ICameraSceneNode::setFarValue );
    _farValueAccessor( 2000.f ); // default value

    _targetAccessor = coca::makeConditionalAccessor<irr::scene::ICameraSceneNode,irr::core::vector3df>(
                       ( irr::scene::ICameraSceneNode* )0,
                       &irr::scene::ICameraSceneNode::getTarget,
                       &irr::scene::ICameraSceneNode::setTarget );
    _targetAccessor( irr::core::vector3df( 1, 0, 0 ) ); // default value

    _upVectorAccessor = coca::makeConditionalAccessor<irr::scene::ICameraSceneNode,irr::core::vector3df>(
                       ( irr::scene::ICameraSceneNode* )0,
                       &irr::scene::ICameraSceneNode::getUpVector,
                       &irr::scene::ICameraSceneNode::setUpVector );
    _upVectorAccessor( irr::core::vector3df( 0, 1, 0 ) ); // default value
}

CameraSceneNodeComponent::~CameraSceneNodeComponent()
{
    COCA_DEBUG_INFO( "~CameraSceneNodeComponent" );
}

bool CameraSceneNodeComponent::init( coca::INode& node )
{
    if ( !SceneNodeComponent::init( node ) ) { return false; }

    coca::IAttribute* attribute = 0;

    attribute = coca::createInputOutputAttribute<bool>( _inputReceiverEnabledAccessor );
    node.addAttribute( "inputReceiverEnabled", attribute );

    attribute = coca::createInputOutputAttribute<bool>( _bindTargetAndRotationAccessor );
    node.addAttribute( "bindTargetAndRotation", attribute );

    attribute = coca::createInputOutputAttribute<irr::f32>( _fovAccessor );
    node.addAttribute( "fov", attribute );

    attribute = coca::createInputOutputAttribute<irr::f32>( _aspectRationAccessor );
    node.addAttribute( "aspectRatio", attribute );

    attribute = coca::createInputOutputAttribute<irr::f32>( _nearValueAccessor );
    node.addAttribute( "nearClipPlane", attribute );

    attribute = coca::createInputOutputAttribute<irr::f32>( _farValueAccessor );
    node.addAttribute( "farClipPlane", attribute );

    attribute = coca::createInputOutputAttribute<irr::core::vector3df>( _targetAccessor );
    node.addAttribute( "target", attribute );

    attribute = coca::createInputOutputAttribute<irr::core::vector3df>( _upVectorAccessor );
    node.addAttribute( "upVector", attribute );

    _cameraNodeAttribute = coca::createOutputAttribute<irr::scene::ICameraSceneNode*>( coca::makeAccessor( _cameraNode ) );
    node.addAttribute( "camera", _cameraNodeAttribute );

    return true;
}

bool CameraSceneNodeComponent::createAndGrabNode( irr::scene::ISceneManager& manager )
{
    _cameraNode = manager.addCameraSceneNode( _parentAccessor() );
    _node = _cameraNode;
    _node->grab();
    return true;
}

void CameraSceneNodeComponent::onNodeCreated()
{
    setCameraAccessorObject( _cameraNode );
    _cameraNodeAttribute->update();
}

void CameraSceneNodeComponent::onNodeRemoved()
{
    setCameraAccessorObject( 0 );
    _cameraNode = 0;
    _cameraNodeAttribute->update();
}

void CameraSceneNodeComponent::setCameraAccessorObject( irr::scene::ICameraSceneNode* node )
{
    _inputReceiverEnabledAccessor.setObject( node );
    _bindTargetAndRotationAccessor.setObject( node );
    _fovAccessor.setObject( node );
    _aspectRationAccessor.setObject( node );
    _nearValueAccessor.setObject( node );
    _farValueAccessor.setObject( node );
    _targetAccessor.setObject( node );
    _upVectorAccessor.setObject( node );
}

