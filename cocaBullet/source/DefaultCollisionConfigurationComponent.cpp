#include "DefaultCollisionConfigurationComponent.h"
#include <coca/INode.h>
#include <coca/attributes.h>

DefaultCollisionConfigurationComponent::DefaultCollisionConfigurationComponent() :
        _configuration( 0 )
{
}

DefaultCollisionConfigurationComponent::~DefaultCollisionConfigurationComponent()
{
}

bool DefaultCollisionConfigurationComponent::init( coca::INode& node )
{
    coca::IAttribute* attribute = coca::createOutputAttribute<btCollisionConfiguration*>(
                                      coca::makeAccessor( _configuration ) );
    node.addAttribute( "configuration", attribute );

    return true;
}

void DefaultCollisionConfigurationComponent::onEnabled()
{
    COCA_ASSERT_EQUAL( _configuration, ( void* )0 );
    _configuration = new btDefaultCollisionConfiguration();
}

void DefaultCollisionConfigurationComponent::onDisabled()
{
    delete _configuration;
    _configuration = 0;
}
