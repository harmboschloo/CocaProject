// Copyright (C) 2007-2008 Harm Boschloo

#include "VideoDriverComponent.h"
#include "irrlicht_io.h"
#include <coca/INode.h>
#include <coca/attributes.h>

VideoDriverComponent::VideoDriverComponent() :
        _driver( 0 ),
        _clearBackBuffer( true ),
        _clearZBuffer( true ),
        _driverOutput( 0 ),
        _preDrawOutput( 0 ),
        _drawOutput( 0 ),
        _postDrawOutput( 0 )
{

}

VideoDriverComponent::~VideoDriverComponent()
{
}

bool VideoDriverComponent::init( coca::INode& node )
{
    coca::IAttribute* attribute = 0;

    _driverOutput = coca::createOutputAttribute<irr::video::IVideoDriver*>( coca::makeAccessor( _driver ) );
    node.addAttribute( "driver", _driverOutput );

    _preDrawOutput = coca::createOutputAttribute();
    node.addAttribute( "preDraw", _preDrawOutput );

    _drawOutput = coca::createOutputAttribute();
    node.addAttribute( "draw", _drawOutput );

    _postDrawOutput = coca::createOutputAttribute();
    node.addAttribute( "postDraw", _postDrawOutput );

    attribute = coca::createInputAttribute<irr::IrrlichtDevice*>(
                    coca::makeFunction( *this, &VideoDriverComponent::setDevice ) );
    node.addAttribute( "device", attribute );

    attribute = coca::createInputOutputAttribute<irr::video::SColor>( coca::makeAccessor( _backgroundColor ) );
    node.addAttribute( "bg", attribute );

    attribute = coca::createInputOutputAttribute<bool>( coca::makeAccessor( _clearBackBuffer ) );
    node.addAttribute( "clearBackBuffer", attribute );

    attribute = coca::createInputOutputAttribute<bool>( coca::makeAccessor( _clearZBuffer ) );
    node.addAttribute( "clearZBuffer", attribute );

    attribute = coca::createInputAttribute<void>( coca::makeFunction( *this, &VideoDriverComponent::run ) );
    node.addAttribute( "run", attribute );

    return true;
}

void VideoDriverComponent::onEnabled()
{
}

void VideoDriverComponent::onDisabled()
{
}

void VideoDriverComponent::setDevice( irr::IrrlichtDevice* device )
{
    if ( device )
    {
        _driver = device->getVideoDriver();
    }
    else
    {
        _driver = 0;
    }
    _driverOutput->update();
}

void VideoDriverComponent::run()
{
    if ( !_driver ) { return; }

    _preDrawOutput->update();
    _driver->beginScene( _clearBackBuffer, _clearZBuffer, _backgroundColor );
    _drawOutput->update();
    _driver->endScene();
    _postDrawOutput->update();
}
