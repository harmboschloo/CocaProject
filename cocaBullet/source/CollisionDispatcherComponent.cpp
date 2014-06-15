#include "CollisionDispatcherComponent.h"
#include <coca/INode.h>
#include <coca/attributes.h>

CollisionDispatcherComponent::CollisionDispatcherComponent() :
        _configuration( 0 ),
        _dispatcher( 0 ),
        _dispatcherAttribute( 0 )
{
}

CollisionDispatcherComponent::~CollisionDispatcherComponent()
{
    COCA_ASSERT_EQUAL( _dispatcher, ( void* )0 );
}

bool CollisionDispatcherComponent::init( coca::INode& node )
{
    coca::IAttribute* attribute = 0;
    attribute = coca::createInputOutputAttribute<btCollisionConfiguration*>(
                    coca::makeAccessor( *this, &CollisionDispatcherComponent::getConfiguration,
                                        &CollisionDispatcherComponent::setConfiguration ) );
    node.addAttribute( "configuration", attribute );

    _dispatcherAttribute = coca::createOutputAttribute<btDispatcher*>(
                               coca::makeAccessor( _dispatcher ) );
    node.addAttribute( "dispatcher", _dispatcherAttribute );

    return true;
}

void CollisionDispatcherComponent::onEnabled()
{
    COCA_ASSERT_EQUAL( _dispatcher, ( void* )0 );
    if ( _configuration )
    {
        if ( _dispatcher )
        {
            COCA_ASSERT_EQUAL( _dispatcher->getCollisionConfiguration(), _configuration );
        }
        else
        {
            _dispatcher = new btCollisionDispatcher( _configuration );
            _dispatcherAttribute->update();
        }
    }
}

void CollisionDispatcherComponent::onDisabled()
{
    if ( _dispatcher )
    {
        delete _dispatcher;
        _dispatcher = 0;
        _dispatcherAttribute->update();
    }
}

btCollisionConfiguration* CollisionDispatcherComponent::getConfiguration() const
{
    return _configuration;
}

void CollisionDispatcherComponent::setConfiguration( btCollisionConfiguration* configuration )
{
    if ( configuration != _configuration )
    {
        _configuration = configuration;

        if ( _configuration && _dispatcher )
        {
            _dispatcher->setCollisionConfiguration( _configuration );
        }
        else if ( _configuration && !_dispatcher )
        {
            _dispatcher = new btCollisionDispatcher( _configuration );
            _dispatcherAttribute->update();
        }
        else if ( !_configuration && _dispatcher )
        {
            delete _dispatcher;
            _dispatcher = 0;
            _dispatcherAttribute->update();
        }
    }
}
