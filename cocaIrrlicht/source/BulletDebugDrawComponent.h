// Copyright (C) 2007-2008 Harm Boschloo

#ifndef IRR_BULLET_DEBUG_DRAW_COMPONENT_H_INCLUDED
#define IRR_BULLET_DEBUG_DRAW_COMPONENT_H_INCLUDED

#include <irrlicht.h>
#include <coca/IComponent.h>
#include <coca/interfaces/IBulletDebugDrawer.h>

class BulletDebugDrawComponent : public coca::IComponent
{
public:
    BulletDebugDrawComponent();
    ~BulletDebugDrawComponent();

    bool init( coca::INode& node );
    void onEnabled();
    void onDisabled();

private:
    class Drawer : public IBulletDebugDrawer
    {
    public:
        Drawer();

        bool beginDrawing();
        void endDrawing();
        void drawLine( float x1, float y1, float z1, float x2, float y2, float z2,
                       int r, int g, int b );

        irr::video::IVideoDriver* driver;
    };

    Drawer* _drawer;
};


#endif //IRR_BULLET_DEBUG_DRAW_COMPONENT_H_INCLUDED
