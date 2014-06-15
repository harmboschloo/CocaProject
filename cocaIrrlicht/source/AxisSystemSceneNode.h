// Copyright (C) 2007-2008 Harm Boschloo

#ifndef IRR_AXIS_SYSTEM_SCENE_NODE_H_INCLUDED
#define IRR_AXIS_SYSTEM_SCENE_NODE_H_INCLUDED

#include <irrlicht.h>

class AxisSystemSceneNode : public irr::scene::ISceneNode
{
public:
    AxisSystemSceneNode( irr::scene::ISceneNode* parent, irr::scene::ISceneManager* manager, irr::s32 id = -1,
                         const irr::core::vector3df& position = irr::core::vector3df( 0,0,0 ),
                         const irr::core::vector3df& rotation = irr::core::vector3df( 0,0,0 ),
                         const irr::core::vector3df& scale = irr::core::vector3df( 1.0f, 1.0f, 1.0f ) );
    ~AxisSystemSceneNode();

    virtual void OnRegisterSceneNode();
    virtual void render();
    virtual const irr::core::aabbox3df& getBoundingBox() const;
    virtual irr::u32 getMaterialCount() const;
    virtual irr::video::SMaterial& getMaterial( irr::u32 i );

private:
    irr::core::aabbox3df _boundingBox;
    irr::video::SMaterial _material;

};

#endif //IRR_AXIS_SYSTEM_SCENE_NODE_H_INCLUDED
