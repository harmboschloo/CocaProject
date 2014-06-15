// Copyright (C) 2007-2008 Harm Boschloo

#include "SceneManagerComponent.h"
#include <coca/INode.h>
#include <coca/attributes.h>

SceneManagerComponent::SceneManagerComponent() :
        _manager( 0 ),
        _root( 0 ),
        _managerOutput( 0 ),
        _rootOutput( 0 )
{
}

SceneManagerComponent::~SceneManagerComponent()
{
}

bool SceneManagerComponent::init( coca::INode& node )
{
    coca::IAttribute* attribute = 0;

    _managerOutput = coca::createOutputAttribute<irr::scene::ISceneManager*>( coca::makeAccessor( _manager ) );
    node.addAttribute( "manager", _managerOutput );

    _rootOutput = coca::createOutputAttribute<irr::scene::ISceneNode*>( coca::makeAccessor( _root ) );
    node.addAttribute( "root", _rootOutput );

    attribute = coca::createInputAttribute<irr::IrrlichtDevice*>(
                    coca::makeFunction( *this, &SceneManagerComponent::setDevice ) );
    node.addAttribute( "device", attribute );

    attribute = coca::createInputAttribute<void>( coca::makeFunction( *this, &SceneManagerComponent::drawAll ) );
    node.addAttribute( "draw", attribute );

    return true;
}

void SceneManagerComponent::onEnabled()
{
}

void SceneManagerComponent::onDisabled()
{
}

void SceneManagerComponent::setDevice( irr::IrrlichtDevice* device )
{
    COCA_DEBUG_INFO( "SceneManagerComponent::setDevice " << device );
    if ( device )
    {
        _manager = device->getSceneManager();
        _root = _manager->getRootSceneNode();
    }
    else
    {
        _manager = 0;
        _root = 0;
    }
    _managerOutput->update();
    _rootOutput->update();
}

void SceneManagerComponent::drawAll()
{
    if ( _manager )
    {
        _manager->drawAll();
    }
}
