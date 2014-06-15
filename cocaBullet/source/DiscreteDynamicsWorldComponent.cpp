#include "DiscreteDynamicsWorldComponent.h"
#include <coca/INode.h>
#include <coca/attributes.h>

DiscreteDynamicsWorldComponent::DiscreteDynamicsWorldComponent() :
        _constraintSolver( 0 ),
        _discreteDynamicsWorld( 0 ),
        _discreteDynamicsWorldAttribute( 0 )
{
}

DiscreteDynamicsWorldComponent::~DiscreteDynamicsWorldComponent()
{
}

bool DiscreteDynamicsWorldComponent::init( coca::INode& node )
{
    if ( !DynamicsWorldComponent::init( node ) ) { return false; }

    coca::IAttribute* attribute = 0;

    attribute = coca::createInputOutputAttribute<btConstraintSolver*>( coca::makeAccessor( _constraintSolver ) );
    attribute->appendUpdateCallback(
        coca::makeFunction( *static_cast<CollisionWorldComponent*>( this ), &CollisionWorldComponent::updateWorld ) );
    node.addAttribute( "constraintSolver", attribute );

    _discreteDynamicsWorldAttribute = coca::createOutputAttribute<btDynamicsWorld*>(
                                          coca::makeAccessor( _discreteDynamicsWorld ) );
    node.addAttribute( "discreteDynamicsWorld", _discreteDynamicsWorldAttribute );

    // all ok
    return true;
}

void DiscreteDynamicsWorldComponent::createWorld()
{
    COCA_DEBUG_INFO( "DiscreteDynamicsWorldComponent::createWorld " << _discreteDynamicsWorld );
    COCA_ASSERT_EQUAL( _discreteDynamicsWorld, ( void* )0 );

    if ( !_dispatcher || !_broadphase || !_constraintSolver || !_collisionConfiguration ) { return; }

    init( new btDiscreteDynamicsWorld( _dispatcher, _broadphase, _constraintSolver,
                                       _collisionConfiguration ) );
}

void DiscreteDynamicsWorldComponent::destroyWorld()
{
    COCA_DEBUG_INFO( "DiscreteDynamicsWorldComponent::destroyWorld " << _discreteDynamicsWorld );

    if ( _discreteDynamicsWorld )
    {
        _discreteDynamicsWorld = 0;
        _discreteDynamicsWorldAttribute->update();
    }

    DynamicsWorldComponent::destroyWorld();
}

void DiscreteDynamicsWorldComponent::init( btDiscreteDynamicsWorld* world )
{
    DynamicsWorldComponent::init( world );
    _discreteDynamicsWorld = world;
    _discreteDynamicsWorldAttribute->update();
}

