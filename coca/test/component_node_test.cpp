#include "component_node_test.h"
#include "test_components.h"
#include <coca/coca.h>

void testComponentNode()
{
    COCA_INFO( "TestComponent::testComponentCtorCount: " << TestComponent::testComponentCtorCount );
    COCA_INFO( "TestComponent::testComponentDtorCount: " << TestComponent::testComponentDtorCount );
    COCA_REQUIRE_EQUAL( TestComponent::testComponentCtorCount, TestComponent::testComponentDtorCount );
    TestComponent::testComponentCtorCount = 0;
    TestComponent::testComponentDtorCount = 0;

    coca::IFactory& factory = coca::getFactory();
    COCA_REQUIRE_EQUAL( factory.registerComponent<TestComponent>( "test" ), true );

    coca::NodePtr root( factory.createCompositeNode( "root" ) );
    COCA_REQUIRE_EQUAL( root->getName(), "root" );
    COCA_REQUIRE_NULL( root->getParent() );
    COCA_REQUIRE_EQUAL( root->isEnabled(), true );
    COCA_REQUIRE_EQUAL( root->getDisabledCount(), 0 );
    COCA_REQUIRE_EQUAL( root->getComponentId(), "" );
    COCA_REQUIRE_NULL( root->getComponent() );
    COCA_REQUIRE_EQUAL( root->getAttributes().size(), (size_t)0 );
    COCA_REQUIRE_EQUAL( root->getChildren().size(), (size_t)0 );
    COCA_REQUIRE_EQUAL( &root->getRoot(), root );

    coca::NodePtr child1( factory.createComponentNode( "", "child1", root ) );
    COCA_REQUIRE_EQUAL( child1->getParent(), root );
    COCA_REQUIRE_EQUAL( &child1->getRoot(), root );
    COCA_REQUIRE_EQUAL( root->getChildren().size(), (size_t)1 );
    COCA_REQUIRE_EQUAL( root->getChildren()[0], child1 );

    coca::NodePtr child2( factory.createComponentNode( "test", "child2", root ) );
    COCA_REQUIRE( child2 );
    TestComponent* test2 = dynamic_cast<TestComponent*>( child2->getComponent() );
    COCA_REQUIRE( test2 );
    coca::IAttribute* attribute = child2->findAttribute( "initCount" );
    COCA_REQUIRE( attribute );
    COCA_REQUIRE_EQUAL( child2->getComponentId(), "test" );
    COCA_REQUIRE_EQUAL( child2->getComponent(), test2 );
    COCA_REQUIRE_EQUAL( child2->getParent(), root );
    COCA_REQUIRE_EQUAL( &child2->getRoot(), root );
    COCA_REQUIRE_EQUAL( root->getChildren().size(), (size_t)2 );
    COCA_REQUIRE_EQUAL( root->getChildren()[1], child2 );
    COCA_REQUIRE_EQUAL( test2->initCount, 1 );
    COCA_REQUIRE_EQUAL( test2->enabledCount, 1 );
    COCA_REQUIRE_EQUAL( test2->disabledCount, 0 );
    COCA_REQUIRE_EQUAL( TestComponent::testComponentCtorCount, 1 );
    COCA_REQUIRE_EQUAL( attribute->isEnabled(), true );
    COCA_REQUIRE_EQUAL( attribute->isSetEnabled(), true );

    root->disable( false );
    COCA_REQUIRE_EQUAL( root->isEnabled(), false );
    COCA_REQUIRE_EQUAL( root->getDisabledCount(), 1 );
    COCA_REQUIRE_EQUAL( child1->isEnabled(), false );
    COCA_REQUIRE_EQUAL( child1->getDisabledCount(), 1 );
    COCA_REQUIRE_EQUAL( child2->isEnabled(), false );
    COCA_REQUIRE_EQUAL( child2->getDisabledCount(), 1 );
    root->disable();
    COCA_REQUIRE_EQUAL( root->isEnabled(), false );
    COCA_REQUIRE_EQUAL( root->getDisabledCount(), 2 );
    COCA_REQUIRE_EQUAL( child1->isEnabled(), false );
    COCA_REQUIRE_EQUAL( child1->getDisabledCount(), 2 );
    COCA_REQUIRE_EQUAL( child2->isEnabled(), false );
    COCA_REQUIRE_EQUAL( child2->getDisabledCount(), 2 );
    COCA_REQUIRE_EQUAL( test2->enabledCount, 1 );
    COCA_REQUIRE_EQUAL( test2->disabledCount, 1 );
    COCA_REQUIRE_EQUAL( attribute->isEnabled(), false );
    COCA_REQUIRE_EQUAL( attribute->isSetEnabled(), true );
    attribute->setEnabled( false );
    COCA_REQUIRE_EQUAL( attribute->isEnabled(), false );
    COCA_REQUIRE_EQUAL( attribute->isSetEnabled(), false );

    root->enable( false );
    COCA_REQUIRE_EQUAL( root->isEnabled(), false );
    COCA_REQUIRE_EQUAL( root->getDisabledCount(), 1 );
    COCA_REQUIRE_EQUAL( child1->isEnabled(), false );
    COCA_REQUIRE_EQUAL( child1->getDisabledCount(), 2 );
    COCA_REQUIRE_EQUAL( child2->isEnabled(), false );
    COCA_REQUIRE_EQUAL( child2->getDisabledCount(), 2 );
    COCA_REQUIRE_EQUAL( test2->enabledCount, 1 );
    COCA_REQUIRE_EQUAL( test2->disabledCount, 1 );
    child2->disable();
    COCA_REQUIRE_EQUAL( child2->isEnabled(), false );
    COCA_REQUIRE_EQUAL( child2->getDisabledCount(), 3 );
    COCA_REQUIRE_EQUAL( test2->enabledCount, 1 );
    COCA_REQUIRE_EQUAL( test2->disabledCount, 1 );
    root->enable();
    COCA_REQUIRE_EQUAL( root->isEnabled(), true );
    COCA_REQUIRE_EQUAL( root->getDisabledCount(), 0 );
    COCA_REQUIRE_EQUAL( child1->isEnabled(), false );
    COCA_REQUIRE_EQUAL( child1->getDisabledCount(), 1 );
    COCA_REQUIRE_EQUAL( child2->isEnabled(), false );
    COCA_REQUIRE_EQUAL( child2->getDisabledCount(), 2 );
    COCA_REQUIRE_EQUAL( test2->enabledCount, 1 );
    COCA_REQUIRE_EQUAL( test2->disabledCount, 1 );

    root->enable();
    COCA_REQUIRE_EQUAL( root->isEnabled(), true );
    COCA_REQUIRE_EQUAL( root->getDisabledCount(), 0 );
    COCA_REQUIRE_EQUAL( child1->isEnabled(), true );
    COCA_REQUIRE_EQUAL( child1->getDisabledCount(), 0 );
    COCA_REQUIRE_EQUAL( child2->isEnabled(), false );
    COCA_REQUIRE_EQUAL( child2->getDisabledCount(), 1 );
    COCA_REQUIRE_EQUAL( test2->enabledCount, 1 );
    COCA_REQUIRE_EQUAL( test2->disabledCount, 1 );
    child2->enable();
    COCA_REQUIRE_EQUAL( test2->enabledCount, 2 );
    COCA_REQUIRE_EQUAL( test2->disabledCount, 1 );
    COCA_REQUIRE_EQUAL( attribute->isEnabled(), false );
    COCA_REQUIRE_EQUAL( attribute->isSetEnabled(), false );
    attribute->setEnabled( true );
    COCA_REQUIRE_EQUAL( attribute->isEnabled(), true );
    COCA_REQUIRE_EQUAL( attribute->isSetEnabled(), true );
    child2->setAttributesEnabled( false );
    COCA_REQUIRE_EQUAL( attribute->isEnabled(), false );
    COCA_REQUIRE_EQUAL( attribute->isSetEnabled(), false );
    child2->setAttributesEnabled( true );
    COCA_REQUIRE_EQUAL( attribute->isEnabled(), true );
    COCA_REQUIRE_EQUAL( attribute->isSetEnabled(), true );

    child2->resetComponent();
    COCA_REQUIRE_EQUAL( TestComponent::testComponentDtorCount, 1 );
    COCA_REQUIRE_EQUAL( root->getComponentId(), "" );
    COCA_REQUIRE_NULL( root->getComponent() );
    COCA_REQUIRE_EQUAL( root->getAttributes().size(), (size_t)0 );
    child2->disable();
    child2->resetComponent( "test" );
    COCA_REQUIRE_EQUAL( TestComponent::testComponentCtorCount, 2 );
    COCA_REQUIRE_EQUAL( child2->getComponentId(), "test" );
    COCA_REQUIRE( child2->getComponent() );
    attribute = child2->findAttribute( "initCount" );
    COCA_REQUIRE( attribute );
    COCA_REQUIRE_EQUAL( attribute->isEnabled(), false );
    COCA_REQUIRE_EQUAL( attribute->isSetEnabled(), true );
    child2->enable();
    COCA_REQUIRE_EQUAL( attribute->isEnabled(), true );
    COCA_REQUIRE_EQUAL( attribute->isSetEnabled(), true );

    root->moveDown( child1 );
    COCA_REQUIRE_EQUAL( root->getChildren()[0], child2 );
    COCA_REQUIRE_EQUAL( root->getChildren()[1], child1 );

    root->moveUp( child1 );
    COCA_REQUIRE_EQUAL( root->getChildren()[0], child1 );
    COCA_REQUIRE_EQUAL( root->getChildren()[1], child2 );

    root->moveUp( child1 );
    COCA_REQUIRE_EQUAL( root->getChildren()[0], child1 );
    COCA_REQUIRE_EQUAL( root->getChildren()[1], child2 );

    root->moveDown( child2 );
    COCA_REQUIRE_EQUAL( root->getChildren()[0], child1 );
    COCA_REQUIRE_EQUAL( root->getChildren()[1], child2 );

    child1.reset();
    child2.reset();
    COCA_REQUIRE_EQUAL( TestComponent::testComponentDtorCount, 1 );
    root.reset();
    COCA_REQUIRE_EQUAL( TestComponent::testComponentDtorCount, 2 );

    factory.clearComponentRegistrations();
}
