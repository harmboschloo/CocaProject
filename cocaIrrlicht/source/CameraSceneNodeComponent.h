// Copyright (C) 2007-2008 Harm Boschloo

#ifndef IRR_CAMERA_SCENE_NODE_COMPONENT_H_INCLUDED
#define IRR_CAMERA_SCENE_NODE_COMPONENT_H_INCLUDED

#include "SceneNodeComponent.h"

class CameraSceneNodeComponent : public SceneNodeComponent
{
public:
    CameraSceneNodeComponent();
    virtual ~CameraSceneNodeComponent();

    virtual bool init( coca::INode& node );

    virtual bool createAndGrabNode( irr::scene::ISceneManager& manager );
    virtual void onNodeCreated();
    virtual void onNodeRemoved();

protected:
    void setCameraAccessorObject( irr::scene::ICameraSceneNode* node );

    irr::scene::ICameraSceneNode* _cameraNode;
    coca::ConditionalAccessor<irr::scene::ICameraSceneNode,bool> _inputReceiverEnabledAccessor;
    coca::ConditionalAccessor<irr::scene::ICameraSceneNode,bool> _bindTargetAndRotationAccessor;
    coca::ConditionalAccessor<irr::scene::ICameraSceneNode,irr::f32> _fovAccessor;
    coca::ConditionalAccessor<irr::scene::ICameraSceneNode,irr::f32> _aspectRationAccessor;
    coca::ConditionalAccessor<irr::scene::ICameraSceneNode,irr::f32> _nearValueAccessor;
    coca::ConditionalAccessor<irr::scene::ICameraSceneNode,irr::f32> _farValueAccessor;
    coca::ConditionalAccessor<irr::scene::ICameraSceneNode,irr::core::vector3df> _targetAccessor;
    coca::ConditionalAccessor<irr::scene::ICameraSceneNode,irr::core::vector3df> _upVectorAccessor;
    coca::IAttribute* _cameraNodeAttribute;

};

#endif //IRR_CAMERA_SCENE_NODE_COMPONENT_H_INCLUDED
