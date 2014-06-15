// Copyright (C) 2007-2008 Harm Boschloo

#include "BulletDebugDrawComponent.h"
#include <coca/INode.h>
#include <coca/attributes.h>

BulletDebugDrawComponent::BulletDebugDrawComponent() :
        _drawer( new Drawer() )
{
}

BulletDebugDrawComponent::~BulletDebugDrawComponent()
{
    delete _drawer;
}

bool BulletDebugDrawComponent::init( coca::INode& node )
{
    coca::IAttribute* attribute = 0;

    attribute = coca::createInputOutputAttribute<irr::video::IVideoDriver*>( coca::makeAccessor( _drawer->driver ) );
    node.addAttribute( "driver", attribute );

    attribute = coca::createOutputAttribute<IBulletDebugDrawer*>( coca::makeAccessor( _drawer ) );
    node.addAttribute( "drawer", attribute );

    return true;
}

void BulletDebugDrawComponent::onEnabled()
{
}

void BulletDebugDrawComponent::onDisabled()
{
}

BulletDebugDrawComponent::Drawer::Drawer() :
        driver( 0 )
{

}

bool BulletDebugDrawComponent::Drawer::beginDrawing()
{
    if ( !driver ) { return false; }

    irr::video::SMaterial material;
    material.Lighting = false;
    driver->setMaterial( material );
    driver->setTransform( irr::video::ETS_WORLD, irr::core::matrix4::EM4CONST_IDENTITY );

    return true;
}

void BulletDebugDrawComponent::Drawer::endDrawing()
{

}

void BulletDebugDrawComponent::Drawer::drawLine( float x1, float y1, float z1,
        float x2, float y2, float z2, int r, int g, int b )
{
    driver->draw3DLine( irr::core::vector3df( x1,y1,z1 ), irr::core::vector3df( x2,y2,z2 ),
                        irr::video::SColor( 255,r,g,b ) );
}
