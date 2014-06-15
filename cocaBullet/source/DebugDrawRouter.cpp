#include "DebugDrawRouter.h"
#include "IBulletDebugDrawer.h"
#include "bullet_io.h"
#include <coca/log/log.h>
#include <coca/utils/test.h>

DebugDrawRouter::DebugDrawRouter() :
        _world( 0 ),
        _drawer( 0 )
{
}

DebugDrawRouter::~DebugDrawRouter()
{
}

btDynamicsWorld* DebugDrawRouter::getWorld() const
{
    return _world;
}

void DebugDrawRouter::setWorld( btDynamicsWorld* world )
{
    if ( _world ) { _world->setDebugDrawer( 0 ); }
    _world = world;
    if ( _world ) { _world->setDebugDrawer( this ); }
}

IBulletDebugDrawer* DebugDrawRouter::getDrawer() const
{
    return _drawer;
}

void DebugDrawRouter::setDrawer( IBulletDebugDrawer* drawer )
{
    _drawer = drawer;
}

void DebugDrawRouter::drawLine( const btVector3& from,const btVector3& to, const btVector3& color )
{
    COCA_ASSERT( _drawer );

    _drawer->drawLine( from.getX(), from.getY(), from.getZ(), to.getX(), to.getY(), to.getZ(),
                       static_cast<int>( color.getX() ), static_cast<int>( color.getY() ),
                       static_cast<int>( color.getZ() ) );
}

void DebugDrawRouter::drawBox( const btVector3& boxMin, const btVector3& boxMax, const btVector3& color, btScalar alpha )
{
    COCA_DEBUG_INFO( "drawBox" );
}

void DebugDrawRouter::drawSphere( const btVector3& p, btScalar radius, const btVector3& color )
{
    COCA_DEBUG_INFO( "drawSphere" );
}

void DebugDrawRouter::drawContactPoint( const btVector3& pointOnB, const btVector3& normalOnB,
                                        btScalar distance, int lifeTime,const btVector3& color )
{
    drawLine( pointOnB, ( pointOnB+( normalOnB/10 ) ), color );
}

void DebugDrawRouter::reportErrorWarning( const char* warningString )
{
    COCA_DEBUG_WARNING( warningString );
}

void DebugDrawRouter::draw3dText( const btVector3& location, const char* textString )
{
    COCA_DEBUG_INFO( textString );
}

void DebugDrawRouter::setDebugMode( int debugMode )
{
    _debugMode.reset( debugMode );
}

int DebugDrawRouter::getDebugMode() const
{
    return _debugMode.get();
}

bool DebugDrawRouter::getDrawWireframe() const
{
    return _debugMode.isSet( btIDebugDraw::DBG_DrawWireframe );
}

void DebugDrawRouter::setDrawWireframe( bool drawWireframe )
{
    _debugMode.set( btIDebugDraw::DBG_DrawWireframe, drawWireframe );
}

bool DebugDrawRouter::getDrawAabb() const
{
    return _debugMode.isSet( btIDebugDraw::DBG_DrawAabb );
}

void DebugDrawRouter::setDrawAabb( bool drawAabb )
{
    _debugMode.set( btIDebugDraw::DBG_DrawAabb, drawAabb );
}

bool DebugDrawRouter::getDrawContactPoints() const
{
    return _debugMode.isSet( btIDebugDraw::DBG_DrawContactPoints );
}

void DebugDrawRouter::setDrawContactPoints( bool drawContactPoints )
{
    _debugMode.set( btIDebugDraw::DBG_DrawContactPoints, drawContactPoints );
}

bool DebugDrawRouter::getNoDeactivation() const
{
    return _debugMode.isSet( btIDebugDraw::DBG_NoDeactivation );
}

void DebugDrawRouter::setNoDeactivation( bool noDeactivation )
{
    _debugMode.set( btIDebugDraw::DBG_NoDeactivation, noDeactivation );
}

bool DebugDrawRouter::getDrawConstraints() const
{
    return _debugMode.isSet( btIDebugDraw::DBG_DrawConstraints );
}

void DebugDrawRouter::setDrawConstraints( bool drawConstraints )
{
    _debugMode.set( btIDebugDraw::DBG_DrawConstraints, drawConstraints );
}

bool DebugDrawRouter::getDrawConstraintLimits() const
{
    return _debugMode.isSet( btIDebugDraw::DBG_DrawConstraintLimits );
}

void DebugDrawRouter::setDrawConstraintLimits( bool drawConstraintLimits )
{
    _debugMode.set( btIDebugDraw::DBG_DrawConstraintLimits, drawConstraintLimits );
}

void DebugDrawRouter::draw()
{
    if ( _drawer && _world )
    {
        if ( _drawer->beginDrawing() )
        {
            _world->debugDrawWorld();
            _drawer->endDrawing();
        }
    }
}
