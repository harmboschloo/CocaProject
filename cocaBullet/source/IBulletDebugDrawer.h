#ifndef I_BULLET_DEBUG_DRAWER_H_INCLUDED
#define I_BULLET_DEBUG_DRAWER_H_INCLUDED

class IBulletDebugDrawer
{
public:
    virtual ~IBulletDebugDrawer() {};

    virtual bool beginDrawing() = 0;
    virtual void endDrawing() = 0;

    virtual void drawLine( float x1, float y1, float z1, float x2, float y2, float z2,
                           int r, int g, int b ) = 0;

};

#endif // I_BULLET_DEBUG_DRAWER_H_INCLUDED
