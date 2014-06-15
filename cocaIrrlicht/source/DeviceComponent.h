#ifndef IRR_DEVICE_COMPONENT_H_INCLUDED
#define IRR_DEVICE_COMPONENT_H_INCLUDED

#include <irrlicht.h>
#include <coca/IComponent.h>
#include <coca/attributes/IAttribute.h>

class DeviceComponent : public coca::IComponent
{
public:
    DeviceComponent();
    ~DeviceComponent();

    bool init( coca::INode& node );
    void onEnabled();
    void onDisabled();

    void createDevice();
    void destroyDevice();
    void updateDevice();

    void run();

    bool isWindowActive() const;

private:
    irr::video::E_DRIVER_TYPE _driverType;
    irr::core::dimension2du _size;
    irr::u32 _bits;
    bool _fullscreen;
    bool _stencilBuffer;
    bool _verticalSync;
    bool _windowActive;
    irr::IrrlichtDevice* _device;
    coca::IAttribute* _deviceOutput;
    coca::IAttribute* _runAttribute;
    coca::IAttribute* _windowActiveOutput;
};

#endif // IRR_DEVICE_COMPONENT_H_INCLUDED
