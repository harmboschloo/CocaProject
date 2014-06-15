// Copyright (C) 2007-2008 Harm Boschloo

#ifndef IRR_VIDEO_DRIVER_COMPONENT_H_INCLUDED
#define IRR_VIDEO_DRIVER_COMPONENT_H_INCLUDED

#include <irrlicht.h>
#include <coca/IComponent.h>
#include <coca/attributes/IAttribute.h>

class VideoDriverComponent : public coca::IComponent
{
public:
    VideoDriverComponent();
    ~VideoDriverComponent();

    bool init( coca::INode& node );
    void onEnabled();
    void onDisabled();

    void setDevice( irr::IrrlichtDevice* device );
    void run();

private:
    irr::video::IVideoDriver* _driver;
    irr::video::SColor _backgroundColor;
    bool _clearBackBuffer;
    bool _clearZBuffer;
    coca::IAttribute* _driverOutput;
    coca::IAttribute* _preDrawOutput;
    coca::IAttribute* _drawOutput;
    coca::IAttribute* _postDrawOutput;
};

#endif //IRR_VIDEO_DRIVER_COMPONENT_H_INCLUDED
