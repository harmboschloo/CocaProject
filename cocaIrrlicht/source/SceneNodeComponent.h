// Copyright (C) 2007-2008 Harm Boschloo

#ifndef IRR_SCENE_NODE_COMPONENT_H_INCLUDED
#define IRR_SCENE_NODE_COMPONENT_H_INCLUDED

#include <irrlicht.h>
#include <coca/IComponent.h>
#include <coca/attributes/IAttribute.h>
#include <coca/utils/function/ConditionalAccessor.h>

class SceneNodeComponent : public coca::IComponent
{
public:
    SceneNodeComponent( bool createNodeOutput = true );
    virtual ~SceneNodeComponent();

    virtual bool init( coca::INode& node );
    virtual void onEnabled();
    virtual void onDisabled();

    virtual bool createAndGrabNode( irr::scene::ISceneManager& manager );
    virtual void onNodeCreated() {};
    virtual void onNodeRemoved() {};

protected:
    void onParentChanged();
    void createNode();
    void dropNode();
    void setAccessorObject( irr::scene::ISceneNode* node );
    void updateNodeOutput();

    bool _createNodeOutput;
    coca::ConditionalAccessor<irr::scene::ISceneNode,irr::scene::ISceneNode*> _parentAccessor;
    coca::ConditionalAccessor<irr::scene::ISceneNode,bool> _visibleAccessor;
    coca::ConditionalAccessor<irr::scene::ISceneNode,irr::core::vector3df> _positionAccessor;
    coca::ConditionalAccessor<irr::scene::ISceneNode,irr::core::vector3df> _rotationAccessor;
    coca::ConditionalAccessor<irr::scene::ISceneNode,irr::core::vector3df> _scaleAccessor;
    irr::scene::ISceneNode* _node;
    coca::IAttribute* _nodeAttribute;
};

#endif //IRR_SCENE_NODE_COMPONENT_H_INCLUDED
