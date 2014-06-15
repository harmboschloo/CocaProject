#ifndef COCA_TEST_COMPONENTS_H_INCLUDED
#define COCA_TEST_COMPONENTS_H_INCLUDED

#include <coca/coca.h>

class TestComponent : public coca::IComponent
{
public:
    static int testComponentCtorCount;
    static int testComponentDtorCount;

    TestComponent() :
        initCount( 0 ),
        enabledCount( 0 ),
        disabledCount( 0 )
    {
        ++testComponentCtorCount;
    }

    virtual ~TestComponent()
    {
        ++testComponentDtorCount;
    }

    virtual bool init( coca::INode& node )
    {
        ++initCount;

        node.addAttribute( "initCount", coca::createInputOutputAttribute<int>( coca::makeAccessor( initCount ) ) );
        node.addAttribute( "enabledCount", coca::createInputOutputAttribute<int>( coca::makeAccessor( enabledCount ) ) );
        node.addAttribute( "disabledCount", coca::createInputOutputAttribute<int>( coca::makeAccessor( disabledCount ) ) );

        COCA_REQUIRE_EQUAL( node.getAttributes().size(), (size_t)3 );
        COCA_REQUIRE( node.findAttribute( "initCount" ) );
        COCA_REQUIRE( node.findAttribute( "enabledCount" ) );
        COCA_REQUIRE( node.findAttribute( "disabledCount" ) );
        COCA_REQUIRE_NULL( node.findAttribute( "bla" ) );

        return true;
    }

    virtual void onEnabled()
    {
        ++enabledCount;
    }

    virtual void onDisabled()
    {
        ++disabledCount;
    }

    int initCount;
    int enabledCount;
    int disabledCount;
};

#endif // COCA_TEST_COMPONENTS_H_INCLUDED
