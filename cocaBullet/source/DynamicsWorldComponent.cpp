#include "DynamicsWorldComponent.h"
#include "bullet_io.h"
#include <coca/INode.h>
#include <coca/attributes.h>

DynamicsWorldComponent::DynamicsWorldComponent() :
        _timeStep( btScalar( 1. )/btScalar( 60. ) ),
        _rigidBodyAttribute( 0 ),
        _dynamicsWorld( 0 ),
        _dynamicsWorldAttribute( 0 ),
        _simulateStepAttribute( 0 ),
        _gravityAccessor( 0 )
{
    _gravityAccessor = coca::makeConditionalAccessor<btDynamicsWorld,btVector3>(
                           ( btDynamicsWorld* )0, &btDynamicsWorld::getGravity,
                           &btDynamicsWorld::setGravity );
    _gravityAccessor( btVector3( 0, 0 , 0 ) ); // default value
}

DynamicsWorldComponent::~DynamicsWorldComponent()
{
}

bool DynamicsWorldComponent::init( coca::INode& node )
{
    if ( !CollisionWorldComponent::init( node ) ) { return false; }

    coca::IAttribute* attribute = 0;

    attribute = coca::createInputOutputAttribute<btScalar>( coca::makeAccessor( _timeStep ) );
    node.addAttribute( "timeStep", attribute );

    attribute = coca::createInputOutputAttribute<btVector3>( _gravityAccessor );
    node.addAttribute( "gravity", attribute );

    _rigidBodyAttribute = coca::createInputAttribute( coca::makeFunction(
                              *this, &DynamicsWorldComponent::onRigidBodyEvent ), true );
    node.addAttribute( "rigidBodies", _rigidBodyAttribute );

    _dynamicsWorldAttribute = coca::createOutputAttribute<btDynamicsWorld*>( coca::makeAccessor( _dynamicsWorld ) );
    node.addAttribute( "dynamicsWorld", _dynamicsWorldAttribute );

    _simulateStepAttribute = coca::createInputOutputAttribute(
                                 coca::makeFunction( *this, &DynamicsWorldComponent::simulateStep ) );
    node.addAttribute( "simulateStep", _simulateStepAttribute );
    _simulateStepAttribute->setAutoUpdate( false );

    // all ok
    return true;
}

void DynamicsWorldComponent::destroyWorld()
{
    COCA_DEBUG_INFO( "DynamicsWorldComponent::destroyWorld " << _dynamicsWorld );

    if ( _dynamicsWorld )
    {
        _dynamicsWorld = 0;
        setAccessorObject(( btDynamicsWorld* )0 );
        _dynamicsWorldAttribute->update();
    }

    CollisionWorldComponent::destroyWorld();
}

void DynamicsWorldComponent::init( btDynamicsWorld* world )
{
    CollisionWorldComponent::init( world );
    _dynamicsWorld = world;
    setAccessorObject( _dynamicsWorld );
    addRigidBodies();
    _dynamicsWorldAttribute->update();
}

void DynamicsWorldComponent::setAccessorObject( btDynamicsWorld* world )
{
    _gravityAccessor.setObject( world );
}

void DynamicsWorldComponent::simulateStep()
{
    if ( !_dynamicsWorld ) { return; }

    if ( _dynamicsWorld->stepSimulation( _timeStep ) > 0 )
    {
        _simulateStepAttribute->update();
    }
}

bool DynamicsWorldComponent::onRigidBodyEvent( const coca::AttributeEvent<btRigidBody*>& event )
{
    switch ( event.getType() )
    {
    case coca::E_SOURCE_ATTACH_EVENT:
        addRigidBody( event.getSource() );
        break;
    case coca::E_SOURCE_DETACH_EVENT:
        removeRigidBody( event.getSource() );
        break;
    case coca::E_SOURCE_UPDATE_EVENT:
        updateRigidBody( event.getSource() );
        break;
    case coca::E_STRING_UPDATE_EVENT:
    default:
        return false;
    }

    return true;
}

void DynamicsWorldComponent::addRigidBodies()
{
    if ( !_dynamicsWorld ) { return; }

    RigidBodyMap::iterator it;
    for ( it = _rigidBodies.begin(); it != _rigidBodies.end(); ++it )
    {
        addRigidBody( it->second );
    }
}

void DynamicsWorldComponent::addRigidBody( coca::AOutputAttribute<btRigidBody*>* source )
{
    COCA_ASSERT( source );
    btRigidBody* body = source->getValue();
    _rigidBodies[source] = body;
    addRigidBody( body );
}

void DynamicsWorldComponent::addRigidBody( btRigidBody* body )
{
    if ( body && _dynamicsWorld )
    {
        COCA_DEBUG_INFO( "Adding rigid body " << body );
        _dynamicsWorld->addRigidBody( body );
    }
}

void DynamicsWorldComponent::removeRigidBody( coca::AOutputAttribute<btRigidBody*>* source )
{
    COCA_ASSERT( source );
    _rigidBodies.erase( source );
    removeRigidBody( source->getValue() );
}

void DynamicsWorldComponent::removeRigidBody( btRigidBody* body )
{
    if ( body && _dynamicsWorld )
    {
        COCA_DEBUG_INFO( "Removing rigid body " << body );
        _dynamicsWorld->removeRigidBody( body );
    }
}

void DynamicsWorldComponent::updateRigidBody( coca::AOutputAttribute<btRigidBody*>* source )
{
    COCA_DEBUG_INFO( "updateRigidBody " << source );

    COCA_ASSERT( source );
    btRigidBody*& body = _rigidBodies[source];
    removeRigidBody( body );
    body = source->getValue();
    addRigidBody( body );
}
