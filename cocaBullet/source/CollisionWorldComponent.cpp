#include "CollisionWorldComponent.h"
#include <coca/INode.h>
#include <coca/attributes.h>

CollisionWorldComponent::CollisionWorldComponent() :
        _dispatcher( 0 ),
        _collisionConfiguration( 0 ),
        _broadphase( 0 ),
        _collisionWorld( 0 ),
        _collisionObjectAttribute( 0 ),
        _collisionWorldAttribute( 0 ),
        _performDiscreteCollisionDetectionAttribute( 0 )
{
}

CollisionWorldComponent::~CollisionWorldComponent()
{
}

bool CollisionWorldComponent::init( coca::INode& node )
{
    coca::IAttribute* attribute = 0;

    attribute = coca::createInputOutputAttribute<btDispatcher*>( coca::makeAccessor( _dispatcher ) );
    attribute->appendUpdateCallback( coca::makeFunction( *this, &CollisionWorldComponent::updateWorld ) );
    node.addAttribute( "dispatcher", attribute );

    attribute = coca::createInputOutputAttribute<btBroadphaseInterface*>( coca::makeAccessor( _broadphase ) );
    attribute->appendUpdateCallback( coca::makeFunction( *this, &CollisionWorldComponent::updateWorld ) );
    node.addAttribute( "broadphase", attribute );

    attribute = coca::createInputOutputAttribute<btCollisionConfiguration*>( coca::makeAccessor( _collisionConfiguration ) );
    attribute->appendUpdateCallback( coca::makeFunction( *this, &CollisionWorldComponent::updateWorld ) );
    node.addAttribute( "collisionConfiguration", attribute );

    _collisionObjectAttribute = coca::createInputAttribute( coca::makeFunction(
                                    *this, &CollisionWorldComponent::onCollisionObjectEvent ), true );
    node.addAttribute( "collisionObjects", _collisionObjectAttribute );

    _collisionWorldAttribute = coca::createOutputAttribute<btCollisionWorld*>( coca::makeAccessor( _collisionWorld ) );
    node.addAttribute( "collisionWorld", _collisionWorldAttribute );

    _performDiscreteCollisionDetectionAttribute = coca::createInputOutputAttribute(
                coca::makeFunction( *this, &CollisionWorldComponent::performDiscreteCollisionDetection ) );
    node.addAttribute( "performDiscreteCollisionDetection", _performDiscreteCollisionDetectionAttribute );

    // all ok
    return true;
}

void CollisionWorldComponent::onEnabled()
{
    createWorld();
}

void CollisionWorldComponent::onDisabled()
{
    destroyWorld();
}

void CollisionWorldComponent::createWorld()
{
    COCA_DEBUG_INFO( "CollisionWorldComponent::createWorld " << _collisionWorld );
    COCA_ASSERT_EQUAL( _collisionWorld, ( void* )0 );

    if ( !_dispatcher || !_broadphase || !_collisionConfiguration ) { return; }

    init( new btCollisionWorld( _dispatcher, _broadphase, _collisionConfiguration ) );
}

void CollisionWorldComponent::destroyWorld()
{
    COCA_DEBUG_INFO( "CollisionWorldComponent::destroyWorld " << _collisionWorld );

    if ( !_collisionWorld ) { return; }

    setAccessorObject( 0 );
    delete _collisionWorld;
    _collisionWorld = 0;
    _collisionWorldAttribute->update();
}

void CollisionWorldComponent::updateWorld()
{
    COCA_DEBUG_INFO( "CollisionWorldComponent::updateWorld " << _collisionWorld );
    destroyWorld();
    createWorld();
}

void CollisionWorldComponent::init( btCollisionWorld* world )
{
    _collisionWorld = world;
    addCollisionObjects();
    setAccessorObject( world );
    _collisionWorldAttribute->update();
}

void CollisionWorldComponent::setAccessorObject( btCollisionWorld* world )
{
}

void CollisionWorldComponent::performDiscreteCollisionDetection()
{
    if ( _collisionWorld ) { _collisionWorld->performDiscreteCollisionDetection(); }
}

bool CollisionWorldComponent::onCollisionObjectEvent( const coca::AttributeEvent<btCollisionObject*>& event )
{
    switch( event.getType() )
    {
        case coca::E_SOURCE_ATTACH_EVENT:
            addCollisionObject( event.getSource() );
            break;
        case coca::E_SOURCE_DETACH_EVENT:
            removeCollisionObject( event.getSource() );
            break;
        case coca::E_SOURCE_UPDATE_EVENT:
            updateCollisionObject( event.getSource() );
            break;
        case coca::E_STRING_UPDATE_EVENT:
        default:
            return false;
    }

    return true;
}

void CollisionWorldComponent::addCollisionObjects()
{
    if ( !_collisionWorld ) { return; }

    CollisionObjectMap::iterator it;
    for ( it = _collisionObjects.begin(); it != _collisionObjects.end(); ++it )
    {
        addCollisionObject( it->second );
    }
}

void CollisionWorldComponent::addCollisionObject( coca::AOutputAttribute<btCollisionObject*>* source )
{
    COCA_ASSERT( source );
    btCollisionObject* object = source->getValue();
    _collisionObjects[source] = object;
    addCollisionObject( object );
}

void CollisionWorldComponent::addCollisionObject( btCollisionObject* object )
{
    if ( object && _collisionWorld )
    {
        if ( object->getCollisionShape() )
        {
            COCA_DEBUG_INFO( "Adding collision object " << object );
            _collisionWorld->addCollisionObject( object );
        }
        else
        {
            COCA_ERROR( "Can not add collision object to collision world. Object needs a collision shape." );
        }
    }
}

void CollisionWorldComponent::removeCollisionObject( coca::AOutputAttribute<btCollisionObject*>* source )
{
    COCA_ASSERT( source );
    _collisionObjects.erase( source );
    removeCollisionObject( source->getValue() );
}

void CollisionWorldComponent::removeCollisionObject( btCollisionObject* object )
{
    if ( object && _collisionWorld )
    {
        COCA_DEBUG_INFO( "Removing collision object " << object );
        _collisionWorld->removeCollisionObject( object );
    }
}

void CollisionWorldComponent::updateCollisionObject( coca::AOutputAttribute<btCollisionObject*>* source )
{
    COCA_ASSERT( source );
    btCollisionObject*& object = _collisionObjects[source];
    removeCollisionObject( object );
    object = source->getValue();
    addCollisionObject( object );
}


