// Copyright (C) 2007-2008 Harm Boschloo

#ifndef IRR_FPS_CAMERA_SCENE_NODE_COMPONENT_H_INCLUDED
#define IRR_FPS_CAMERA_SCENE_NODE_COMPONENT_H_INCLUDED

#include "CameraSceneNodeComponent.h"

class FPSCameraSceneNodeComponent : public CameraSceneNodeComponent
{
public:
    FPSCameraSceneNodeComponent();
    ~FPSCameraSceneNodeComponent();

    bool init( coca::INode& node );
    bool createAndGrabNode( irr::scene::ISceneManager& manager );

private:
    void updateFPSCameraParameters();

    irr::f32 _rotationSpeed;
    irr::f32 _moveSpeed;
};

#endif //IRR_FPS_CAMERA_SCENE_NODE_COMPONENT_H_INCLUDED
