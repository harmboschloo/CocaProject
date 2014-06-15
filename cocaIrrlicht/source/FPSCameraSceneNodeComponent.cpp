// Copyright (C) 2007-2008 Harm Boschloo

#include "FPSCameraSceneNodeComponent.h"
#include <coca/INode.h>
#include <coca/attributes.h>

FPSCameraSceneNodeComponent::FPSCameraSceneNodeComponent() :
    _rotationSpeed( 100.f ),
    _moveSpeed( 0.02f )
{
}

FPSCameraSceneNodeComponent::~FPSCameraSceneNodeComponent()
{
    COCA_DEBUG_INFO( "~FPSCameraSceneNodeComponent" );
}

bool FPSCameraSceneNodeComponent::init( coca::INode& node )
{
    if ( !CameraSceneNodeComponent::init( node ) ) { return false; }

    coca::IAttribute* attribute = 0;

    attribute = coca::createInputOutputAttribute<irr::f32>( coca::makeAccessor( _rotationSpeed ) );
    attribute->appendUpdateCallback( coca::makeFunction( *this, &FPSCameraSceneNodeComponent::updateFPSCameraParameters ) );
    node.addAttribute( "rotationSpeed", attribute );

    attribute = coca::createInputOutputAttribute<irr::f32>( coca::makeAccessor( _moveSpeed ) );
    attribute->appendUpdateCallback( coca::makeFunction( *this, &FPSCameraSceneNodeComponent::updateFPSCameraParameters ) );
    node.addAttribute( "moveSpeed", attribute );

    return true;
}

bool FPSCameraSceneNodeComponent::createAndGrabNode( irr::scene::ISceneManager& manager )
{
    _cameraNode = manager.addCameraSceneNodeFPS( _parentAccessor(), _rotationSpeed, _moveSpeed );
    _node = _cameraNode;
    _node->grab();
    return true;
}

void FPSCameraSceneNodeComponent::updateFPSCameraParameters()
{
    COCA_DEBUG_INFO( "FPSCameraSceneNodeComponent::updateFPSCameraParameters" );
    if ( _node )
    {
        dropNode();
        createNode();
    }
}
