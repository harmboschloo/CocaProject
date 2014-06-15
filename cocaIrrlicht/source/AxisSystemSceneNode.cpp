// Copyright (C) 2007-2008 Harm Boschloo

#include "AxisSystemSceneNode.h"

AxisSystemSceneNode::AxisSystemSceneNode( irr::scene::ISceneNode* parent, irr::scene::ISceneManager* manager,
        irr::s32 id, const irr::core::vector3df& position, const irr::core::vector3df& rotation,
        const irr::core::vector3df& scale ) :
        irr::scene::ISceneNode( parent, manager, id, position, rotation, scale )
{
    _boundingBox.reset( 0,0,0 );
    _boundingBox.addInternalPoint( 1, 0, 0 );
    _boundingBox.addInternalPoint( 0, 1, 0 );
    _boundingBox.addInternalPoint( 0, 0, 1 );
    _material.Lighting = false;
}

AxisSystemSceneNode::~AxisSystemSceneNode()
{
}

void AxisSystemSceneNode::OnRegisterSceneNode()
{
    if ( IsVisible ) { SceneManager->registerNodeForRendering( this ); }
    ISceneNode::OnRegisterSceneNode();
}

void AxisSystemSceneNode::render()
{
    irr::video::IVideoDriver* driver = SceneManager->getVideoDriver();
    driver->setMaterial( _material );
    //driver->setTransform( irr::video::ETS_WORLD, irr::core::matrix4() );
    driver->setTransform( irr::video::ETS_WORLD, AbsoluteTransformation );
    driver->draw3DLine( irr::core::vector3df( 0,0,0 ), irr::core::vector3df( 1,0,0 ), irr::video::SColor( 255,255,0,0 ) );
    driver->draw3DLine( irr::core::vector3df( 0,0,0 ), irr::core::vector3df( 0,1,0 ), irr::video::SColor( 255,0,255,0 ) );
    driver->draw3DLine( irr::core::vector3df( 0,0,0 ), irr::core::vector3df( 0,0,1 ), irr::video::SColor( 255,0,0,255 ) );

}

const irr::core::aabbox3df& AxisSystemSceneNode::getBoundingBox() const
{
    return _boundingBox;
}

irr::u32 AxisSystemSceneNode::getMaterialCount() const
{
    return 1;
}

irr::video::SMaterial& AxisSystemSceneNode::getMaterial( irr::u32 i )
{
    return _material;
}


