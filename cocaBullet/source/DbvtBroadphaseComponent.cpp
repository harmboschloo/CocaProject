#include "DbvtBroadphaseComponent.h"
#include <coca/INode.h>
#include <coca/attributes.h>

DbvtBroadphaseComponent::DbvtBroadphaseComponent() :
        _broadphase( 0 ),
        _broadphaseAttribute( 0 )
{
}

DbvtBroadphaseComponent::~DbvtBroadphaseComponent()
{
}

bool DbvtBroadphaseComponent::init( coca::INode& node )
{
    _broadphaseAttribute = coca::createOutputAttribute<btBroadphaseInterface*>(
                                      coca::makeAccessor( _broadphase ) );
    node.addAttribute( "broadphase", _broadphaseAttribute );

    return true;
}

void DbvtBroadphaseComponent::onEnabled()
{
    COCA_ASSERT_EQUAL( _broadphase, ( void* )0 );
    _broadphase = new btDbvtBroadphase();
}

void DbvtBroadphaseComponent::onDisabled()
{
    btDbvtBroadphase* broadphase = _broadphase;
    _broadphase = 0;
    _broadphaseAttribute->update();
    delete broadphase;
}
