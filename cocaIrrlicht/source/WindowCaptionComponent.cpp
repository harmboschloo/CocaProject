// Copyright (C) 2007-2008 Harm Boschloo

#include "WindowCaptionComponent.h"
#include <coca/INode.h>
#include <coca/attributes.h>

WindowCaptionComponent::WindowCaptionComponent() :
        _showDriver( false ),
        _showFps( false ),
        _lastFps( 0 ),
        _device( 0 )
{
}

WindowCaptionComponent::~WindowCaptionComponent()
{
}

bool WindowCaptionComponent::init( coca::INode& node )
{
    coca::IAttribute* attribute = 0;

    attribute = coca::createInputAttribute<irr::IrrlichtDevice*>( coca::makeAccessor( _device ) );
    attribute->appendUpdateCallback( coca::makeFunction( *this, &WindowCaptionComponent::updateCaption ) );
    node.addAttribute( "device", attribute );

    attribute = coca::createInputOutputAttribute<std::string>( coca::makeAccessor( _caption ) );
    attribute->appendUpdateCallback( coca::makeFunction( *this, &WindowCaptionComponent::updateCaption ) );
    node.addAttribute( "caption", attribute );

    attribute = coca::createInputOutputAttribute<bool>( coca::makeAccessor( _showDriver ) );
    attribute->appendUpdateCallback( coca::makeFunction( *this, &WindowCaptionComponent::updateCaption ) );
    node.addAttribute( "showDriver", attribute );

    attribute = coca::createInputOutputAttribute<bool>( coca::makeAccessor( _showFps ) );
    attribute->appendUpdateCallback( coca::makeFunction( *this, &WindowCaptionComponent::updateCaption ) );
    node.addAttribute( "showFps", attribute );

    attribute = coca::createInputAttribute<void>( coca::makeFunction( *this, &WindowCaptionComponent::update ) );
    node.addAttribute( "update", attribute );

    return true;
}

void WindowCaptionComponent::onEnabled()
{
}

void WindowCaptionComponent::onDisabled()
{
}

void WindowCaptionComponent::update()
{
    if ( !_device ) { return; }

    int fps = _device->getVideoDriver()->getFPS();
    if ( _lastFps != fps )
    {
        _lastFps = fps;
        updateCaption();
    }
}

void WindowCaptionComponent::updateCaption() const
{
    if ( !_device ) { return; }

    irr::core::stringw caption( _caption.c_str() );

    if ( _showDriver || _showFps )
    {
        caption += " [";
        if ( _showDriver )
        {
            caption += _device->getVideoDriver()->getName();
        }

        if ( _showDriver && _showFps )
        {
            caption += " - ";
        }

        if ( _showFps )
        {
            caption += _lastFps;
            caption += " FPS";
        }
        caption += "]";
    }

    _device->setWindowCaption( caption.c_str() );

}

