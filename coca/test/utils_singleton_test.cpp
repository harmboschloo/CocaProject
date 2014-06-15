#include "utils_singleton_test.h"

// TestSingletonMacro //
/*
COCA_IMPLEMENT_SINGLETON( TestSingletonMacro )

TestSingletonMacro::TestSingletonMacro()
{
    COCA_INFO( "TestSingletonMacro constructor" );
    COCA_REQUIRE_EQUAL( _instance.get(), 0 );
}

TestSingletonMacro::~TestSingletonMacro()
{
    COCA_INFO( "TestSingletonMacro destructor" );
    COCA_REQUIRE_EQUAL( _instance.get(), this );
}

// TestSingletonTemplate //

TestSingletonTemplate::TestSingletonTemplate()
{
    COCA_INFO( "TestSingletonTemplate constructor" );
    COCA_REQUIRE_EQUAL( _instance.get(), 0 );
}

TestSingletonTemplate::~TestSingletonTemplate()
{
    COCA_INFO( "TestSingletonTemplate destructor" );
    COCA_REQUIRE_EQUAL( _instance.get(), this );
}
*/
// testSingleton //
/*
template<typename T>
void testSingleton()
{
    T::destroyInstance();
    T::getInstance();
    T::getInstance();
    T::destroyInstance();
    T::destroyInstance();
    T::getInstance(); // to check destruction on exit, has to be checked manually
}
*/

void testSingleton()
{
    //testSingleton<TestSingletonMacro>();
    //testSingleton<TestSingletonTemplate>();
}
