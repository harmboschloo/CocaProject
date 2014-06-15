#include "DeviceComponent.h"
#include "irrlicht_io.h"
#include <coca/INode.h>
#include <coca/attributes.h>

DeviceComponent::DeviceComponent() :
        _driverType( irr::video::EDT_NULL ),
        _size( 640, 480 ),
        _bits( 16 ),
        _fullscreen( false ),
        _stencilBuffer( false ),
        _verticalSync( false ),
        _windowActive( false ),
        _device( 0 ),
        _deviceOutput( 0 ),
        _runAttribute( 0 ),
        _windowActiveOutput( 0 )
{
}

DeviceComponent::~DeviceComponent()
{
}

bool DeviceComponent::init( coca::INode& node )
{
    coca::IAttribute* attribute = 0;

    attribute = coca::createInputOutputAttribute<irr::video::E_DRIVER_TYPE>( coca::makeAccessor( _driverType ) );
    attribute->appendUpdateCallback( coca::makeFunction( *this, &DeviceComponent::updateDevice ) );
    node.addAttribute( "driver", attribute );

    attribute = coca::createInputOutputAttribute<irr::core::dimension2du>( coca::makeAccessor( _size ) );
    attribute->appendUpdateCallback( coca::makeFunction( *this, &DeviceComponent::updateDevice ) );
    node.addAttribute( "size", attribute );

    attribute = coca::createInputOutputAttribute<irr::u32>( coca::makeAccessor( _bits ) );
    attribute->appendUpdateCallback( coca::makeFunction( *this, &DeviceComponent::updateDevice ) );
    node.addAttribute( "bits", attribute );

    attribute = coca::createInputOutputAttribute<bool>( coca::makeAccessor( _fullscreen ) );
    attribute->appendUpdateCallback( coca::makeFunction( *this, &DeviceComponent::updateDevice ) );
    node.addAttribute( "fullscreen", attribute );

    attribute = coca::createInputOutputAttribute<bool>( coca::makeAccessor( _stencilBuffer ) );
    attribute->appendUpdateCallback( coca::makeFunction( *this, &DeviceComponent::updateDevice ) );
    node.addAttribute( "stencilBuffer", attribute );

    attribute = coca::createInputOutputAttribute<bool>( coca::makeAccessor( _verticalSync ) );
    attribute->appendUpdateCallback( coca::makeFunction( *this, &DeviceComponent::updateDevice ) );
    node.addAttribute( "vsync", attribute );

    _deviceOutput = coca::createOutputAttribute<irr::IrrlichtDevice*>( coca::makeAccessor( _device ) );
    node.addAttribute( "device", _deviceOutput );

    _runAttribute = coca::createInputOutputAttribute( coca::makeFunction( *this, &DeviceComponent::run ) );
    node.addAttribute( "run", _runAttribute );
    _runAttribute->setAutoUpdate( false );

    _windowActiveOutput = coca::createOutputAttribute<bool>( coca::makeAccessor( _windowActive ) );
    node.addAttribute( "windowActive", _windowActiveOutput );

    // all ok
    return true;
}

void DeviceComponent::onEnabled()
{
    createDevice();
}

void DeviceComponent::onDisabled()
{
    destroyDevice();
}

void DeviceComponent::createDevice()
{
    COCA_DEBUG_INFO( "DeviceComponent::createDevice " << _device );
    COCA_ASSERT_EQUAL( _device, (void*)0 );

    COCA_DEBUG_INFO( "Creating device, driver: " << _driverType
                  << ", windowSize: " << _size.Width << "x" << _size.Height
                  << ", bits: " << _bits << ", fullsceen: " << _fullscreen
                  << ", stencil buffer: " << _stencilBuffer << ", vsync: " << _verticalSync );

    _device = irr::createDevice( _driverType, _size, _bits, _fullscreen, _stencilBuffer, _verticalSync );

    if ( _device )
    {
        COCA_DEBUG_INFO( "Created device: " << _device );
        _deviceOutput->update();
    }
    else
    {
        COCA_ERROR( "Could not create irrlicht device! Driver: " << _driverType
                  << ", windowSize: " << _size.Width << "x" << _size.Height
                  << ", bits: " << _bits << ", fullsceen: " << _fullscreen
                  << ", stencil buffer: " << _stencilBuffer << ", vsync: " << _verticalSync );
    }
}

void DeviceComponent::destroyDevice()
{
    COCA_DEBUG_INFO( "DeviceComponent::destroyDevice " << _device );

    if ( !_device ) { return; }

    irr::IrrlichtDevice* device = _device;
    _device->run();
    _device->closeDevice();
    _device = 0;
    _deviceOutput->update();
    if ( !device->drop() )
    {
        COCA_WARNING( "Irrlicht device was not deleted and still has references!" );
    }
}

void DeviceComponent::updateDevice()
{
    COCA_DEBUG_INFO( "DeviceComponent::updateDevice " << _device );
    destroyDevice();
    createDevice();
}

void DeviceComponent::run()
{
    if ( _device )
    {
        if ( _device->run() )
        {
            _runAttribute->update();
        }
        else
        {
            COCA_DEBUG_WARNING( "_device->run() failed!" );
            destroyDevice();
        }
    }

    // window active
    bool windowActive = isWindowActive();
    if ( _windowActive != windowActive )
    {
        _windowActive = windowActive;
        _windowActiveOutput->update();
    }
}

bool DeviceComponent::isWindowActive() const
{
    return ( _device ? _device->isWindowActive() : false );
}
