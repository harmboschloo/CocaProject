#ifndef DEBUG_DRAW_ROUTER_H_INCLUDED
#define DEBUG_DRAW_ROUTER_H_INCLUDED

#include <btBulletDynamicsCommon.h>
#include <coca/utils/Flags.h>

class IBulletDebugDrawer;
class btDynamicsWorld;

class DebugDrawRouter : public btIDebugDraw
{
public:
    DebugDrawRouter();
    ~DebugDrawRouter();

    btDynamicsWorld* getWorld() const;
    void setWorld( btDynamicsWorld* drawer );

    IBulletDebugDrawer* getDrawer() const;
    void setDrawer( IBulletDebugDrawer* world );

    void drawLine( const btVector3& from,const btVector3& to, const btVector3& color );
    void drawBox( const btVector3& boxMin, const btVector3& boxMax, const btVector3& color, btScalar alpha );
    void drawSphere( const btVector3& p, btScalar radius, const btVector3& color );
    void drawContactPoint( const btVector3& PointOnB, const btVector3& normalOnB,
                           btScalar distance, int lifeTime,const btVector3& color );
    void reportErrorWarning( const char* warningString );
    void draw3dText( const btVector3& location, const char* textString );

    void setDebugMode( int debugMode );
    int getDebugMode() const;

    bool getDrawWireframe() const;
    void setDrawWireframe( bool drawWireframe );
    bool getDrawAabb() const;
    void setDrawAabb( bool drawAabb );
    bool getDrawContactPoints() const;
    void setDrawContactPoints( bool drawContactPoints );
    bool getNoDeactivation() const;
    void setNoDeactivation( bool noDeactivation );
    bool getDrawConstraints() const;
    void setDrawConstraints( bool drawConstraints );
    bool getDrawConstraintLimits() const;
    void setDrawConstraintLimits( bool drawConstraintLimits );

    void draw();

private:
    btDynamicsWorld* _world;
    IBulletDebugDrawer* _drawer;
    coca::Flags<int> _debugMode;
};

#endif // DEBUG_DRAW_ROUTER_H_INCLUDED
