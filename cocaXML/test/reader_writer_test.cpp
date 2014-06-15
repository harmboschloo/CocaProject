#include "reader_writer_test.h"
#include "../source/XMLWriter.h"
#include "../source/XMLReader.h"
#include <coca/coca.h>
#include <memory>
#include <sstream>
#include <string>

class TestComponent : public coca::IComponent
{
public:
    static int objectCount;

    TestComponent() :
            input1( 0 ),
            output1( 456 ),
            setting1( 123 )
    {
        ++objectCount;
    }

    ~TestComponent()
    {
        --objectCount;
    }

    bool init( coca::INode& node )
    {
        node.addAttribute( "in1", coca::createInputAttribute<int*>( coca::makeAccessor( input1 ) ) );
        node.addAttribute( "out1", coca::createOutputAttribute<int>( coca::makeAccessor( output1 ) ) );
        node.addAttribute( "set1", coca::createInputOutputAttribute<int>( coca::makeAccessor( setting1 ) ) );
        return true;
    }

    void onEnabled()
    {
    }

    void onDisabled()
    {
    }

    int* input1;
    int output1;
    int setting1;
};

int TestComponent::objectCount = 0;

void registerComponents()
{
    coca::getFactory().registerComponent<TestComponent>( "test" );
}

coca::ISystem* createSystem()
{
    coca::IFactory& factory = coca::getFactory();
    coca::ISystem* system = factory.createSystem();
    system->setDescription( "Hello cocaXML!\nBy H.W. Boschloo" );

    system->setRoot( factory.createCompositeNode( "Root" ) );
    COCA_REQUIRE( system->getRoot() );
    coca::INode* child1 = factory.createComponentNode( "test", "child1", system->getRoot() );
    coca::INode* child2 = factory.createComponentNode( "test", "child2", system->getRoot() );
    TestComponent* child1Component = dynamic_cast<TestComponent*>( child1->getComponent() );
    COCA_REQUIRE( child1Component );
    child1Component->setting1 = 987;

    COCA_REQUIRE( child2->findAttribute( "set1" )->connectSource( *child1->findAttribute( "out1" ) ) );
    //FIXME COCA_REQUIRE_EQUAL( system->getRoot()->addAttribute( "set1 wrap", child2->findAttribute( "set1" ) ), true );

    return system;
}

void testSystem( const coca::ISystem& system )
{
    COCA_REQUIRE_EQUAL( system.getDescription(), "Hello cocaXML!\nBy H.W. Boschloo" );
    COCA_REQUIRE( system.getRoot() );
    COCA_REQUIRE_EQUAL( system.getRoot()->getName(), "Root" );
    COCA_REQUIRE_EQUAL( system.getRoot()->isCompositeNode(), true );
    COCA_REQUIRE_EQUAL( system.getRoot()->getChildren().size(), (size_t)2 );
    COCA_REQUIRE_EQUAL( system.getRoot()->getChildren()[0]->getName(), "child1" );
    COCA_REQUIRE_EQUAL( system.getRoot()->getChildren()[1]->getName(), "child2" );
    COCA_REQUIRE_EQUAL( system.getRoot()->getAttributes().size(), (size_t)0 );
    //FIXME COCA_REQUIRE( system.getRoot()->findAttribute( "set1 wrap" ) );
    //FIXME COCA_REQUIRE_EQUAL( system.getRoot()->findAttribute( "set1 wrap" ),
    //                    system.getRoot()->getChildren()[1]->findAttribute( "set1" ) );

    TestComponent* child1Component = dynamic_cast<TestComponent*>( system.getRoot()->getChildren()[0]->getComponent() );
    TestComponent* child2Component = dynamic_cast<TestComponent*>( system.getRoot()->getChildren()[1]->getComponent() );
    COCA_REQUIRE( child1Component );
    COCA_REQUIRE( child2Component );

    COCA_REQUIRE_EQUAL( child1Component->input1, (void*)0 );
    COCA_REQUIRE_EQUAL( child2Component->input1, (void*)0 );
    COCA_REQUIRE_EQUAL( child1Component->setting1, 987 );
    COCA_REQUIRE_EQUAL( child2Component->setting1, 456 );
}

void testReaderWriter()
{
    registerComponents();

    std::auto_ptr<coca::ISystem> system;
    std::auto_ptr<coca::ISystem> system2;
    std::ostringstream oss;
    std::istringstream iss;
    XMLWriter writer;
    XMLReader reader;

    // version check
    iss.str( "<coca_xml version=\"2\"><system/></coca_xml>" );
    system.reset( reader.readSystem( iss ) );
    COCA_REQUIRE( system.get() );

    // invalid version check
    iss.str( "<coca_xml><system/></coca_xml>" );
    system.reset( reader.readSystem( iss ) );
    COCA_REQUIRE_EQUAL( system.get(), (void*)0 );

    // invalid second root check
    iss.str( "<coca_xml version=\"2\"><system><component_node/><composite_node/></system></coca_xml>" );
    system.reset( reader.readSystem( iss ) );
    COCA_REQUIRE_EQUAL( system.get(), (void*)0 );

    // test system
    system.reset( createSystem() );
    testSystem( *system );

    COCA_REQUIRE_EQUAL( writer.write( *system, oss ), true );
    COCA_REQUIRE_EQUAL( oss.fail(), false );
    std::string systemStr = oss.str();
    COCA_INFO( systemStr );
    iss.str( systemStr );
    COCA_REQUIRE_EQUAL( iss.fail(), false );
    system2.reset( reader.readSystem( iss ) );
    COCA_REQUIRE_EQUAL( iss.fail(), false );
    COCA_REQUIRE( system2.get() );
    testSystem( *system2 );

    oss.str( "" );
    COCA_REQUIRE_EQUAL( oss.fail(), false );
    COCA_REQUIRE_EQUAL( writer.write( *system2, oss ), true );
    COCA_REQUIRE_EQUAL( oss.fail(), false );
    std::string systemStr2 = oss.str();
    COCA_INFO( systemStr2 );
    COCA_REQUIRE_EQUAL( systemStr.size(), systemStr2.size() );
    iss.str( oss.str() );
    COCA_REQUIRE_EQUAL( iss.fail(), false );
    system2.reset( reader.readSystem( iss ) );
    COCA_REQUIRE_EQUAL( iss.fail(), false );
    COCA_REQUIRE( system2.get() );
    testSystem( *system2 );

    system.reset();
    system2.reset();

    COCA_REQUIRE_EQUAL( TestComponent::objectCount, 0 );
}
