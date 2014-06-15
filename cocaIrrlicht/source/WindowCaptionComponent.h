// Copyright (C) 2007-2008 Harm Boschloo

#ifndef IRR_WINDOW_CAPTION_COMPONENT_H_INCLUDED
#define IRR_WINDOW_CAPTION_COMPONENT_H_INCLUDED

#include <irrlicht.h>
#include <coca/IComponent.h>
#include <string>

class WindowCaptionComponent : public coca::IComponent
{
public:
    WindowCaptionComponent();
    ~WindowCaptionComponent();

    bool init( coca::INode& node );
    void onEnabled();
    void onDisabled();

    void update();

private:
    void updateCaption() const;

    std::string _caption;
    bool _showDriver;
    bool _showFps;
    int _lastFps;
    irr::IrrlichtDevice* _device;
};

#endif //IRR_WINDOW_CAPTION_COMPONENT_H_INCLUDED
