// Copyright (C) 2007-2008 Harm Boschloo

#include "AxisSystemComponent.h"
#include "AxisSystemSceneNode.h"

AxisSystemComponent::AxisSystemComponent() :
    SceneNodeComponent( false )
{
}

AxisSystemComponent::~AxisSystemComponent()
{
    COCA_DEBUG_INFO( "~AxisSystemComponent" );
}

bool AxisSystemComponent::createAndGrabNode( irr::scene::ISceneManager& manager )
{
    _node = new AxisSystemSceneNode( _parentAccessor(), &manager );
    // scene nodes are created with reference count 1, so we don't need to grab it here
    return true;
}
