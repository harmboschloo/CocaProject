#include "DebugDrawRouterComponent.h"
#include "DebugDrawRouter.h"
#include <coca/INode.h>
#include <coca/attributes.h>

DebugDrawRouterComponent::DebugDrawRouterComponent() :
        _router( new DebugDrawRouter() )
{
}

DebugDrawRouterComponent::~DebugDrawRouterComponent()
{
    delete _router;
}

bool DebugDrawRouterComponent::init( coca::INode& node )
{
    coca::IAttribute* attribute = 0;

    attribute = coca::createInputOutputAttribute<btDynamicsWorld*>(
                    coca::makeAccessor( *_router, &DebugDrawRouter::getWorld,
                                        &DebugDrawRouter::setWorld ) );
    node.addAttribute( "world", attribute );

    attribute = coca::createInputOutputAttribute<IBulletDebugDrawer*>(
                    coca::makeAccessor( *_router, &DebugDrawRouter::getDrawer,
                                        &DebugDrawRouter::setDrawer ) );
    node.addAttribute( "drawer", attribute );

    attribute = coca::createInputOutputAttribute<bool>(
                    coca::makeAccessor( *_router, &DebugDrawRouter::getDrawWireframe,
                                        &DebugDrawRouter::setDrawWireframe ) );
    node.addAttribute( "drawWireframe", attribute );

    attribute = coca::createInputOutputAttribute<bool>(
                    coca::makeAccessor( *_router, &DebugDrawRouter::getDrawAabb,
                                        &DebugDrawRouter::setDrawAabb ) );
    node.addAttribute( "drawAabb", attribute );

    attribute = coca::createInputOutputAttribute<bool>(
                    coca::makeAccessor( *_router, &DebugDrawRouter::getDrawContactPoints,
                                        &DebugDrawRouter::setDrawContactPoints ) );
    node.addAttribute( "drawContactPoints", attribute );

    attribute = coca::createInputOutputAttribute<bool>(
                    coca::makeAccessor( *_router, &DebugDrawRouter::getNoDeactivation,
                                        &DebugDrawRouter::setNoDeactivation ) );
    node.addAttribute( "noDeactivation", attribute );

    attribute = coca::createInputOutputAttribute<bool>(
                    coca::makeAccessor( *_router, &DebugDrawRouter::getDrawConstraints,
                                        &DebugDrawRouter::setDrawConstraints ) );
    node.addAttribute( "drawConstraints", attribute );

    attribute = coca::createInputOutputAttribute<bool>(
                    coca::makeAccessor( *_router, &DebugDrawRouter::getDrawConstraintLimits,
                                        &DebugDrawRouter::setDrawConstraintLimits ) );
    node.addAttribute( "drawConstraintLimits", attribute );

    attribute = coca::createInputOutputAttribute(
                    coca::makeFunction( *_router, &DebugDrawRouter::draw ) );
    node.addAttribute( "draw", attribute );

    return true;
}

void DebugDrawRouterComponent::onEnabled()
{
}

void DebugDrawRouterComponent::onDisabled()
{
}

