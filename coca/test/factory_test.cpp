#include "factory_test.h"
#include "test_components.h"
#include <coca/coca.h>

void testFactory()
{
    COCA_REQUIRE_EQUAL( TestComponent::testComponentCtorCount, TestComponent::testComponentDtorCount );
    TestComponent::testComponentCtorCount = 0;
    TestComponent::testComponentDtorCount = 0;

    coca::IFactory& factory = coca::getFactory();
    COCA_REQUIRE_EQUAL( factory.registerComponent<TestComponent>( "test" ), true );
    COCA_REQUIRE_EQUAL( factory.registerComponent<TestComponent>( "test" ), false );

    coca::IComponent* component = 0;
    component = factory.createComponent( "unregistered id" );
    COCA_REQUIRE_NULL( component );
    component = factory.createComponent( "test" );
    COCA_REQUIRE( component );
    delete component;
    component = 0;
    coca::NodePtr root( factory.createCompositeNode( "root" ) );
    COCA_REQUIRE( root );
    COCA_REQUIRE_NULL( root->getComponent() );
    COCA_REQUIRE_EQUAL( root->getComponentId(), "" );
    COCA_REQUIRE_EQUAL( root->getName(), "root" );
    coca::NodePtr child( factory.createComponentNode( "test", "child", root ) );
    COCA_REQUIRE( child );
    COCA_REQUIRE( child->getComponent() );
    COCA_REQUIRE_EQUAL( child->getComponentId(), "test" );
    COCA_REQUIRE_EQUAL( child->getName(), "child" );
    COCA_REQUIRE_EQUAL( child->getParent(), root );
    COCA_REQUIRE_EQUAL( &child->getRoot(), root );

    root.reset();
    child.reset();
    COCA_REQUIRE_EQUAL( TestComponent::testComponentCtorCount, 2 );
    COCA_REQUIRE_EQUAL( TestComponent::testComponentDtorCount, 2 );
}
