#include <coca/coca.h>

coca::StdLogMessageHandler _stdLog;
coca::FileLogMessageHandler _fileLog( "test.log" );

void myTest()
{
    // Trivial test example
    COCA_REQUIRE( 123 != 123123 );
    COCA_REQUIRE_EQUAL( 123, 123 );
    COCA_REQUIRE_NULL( (void*)0 );
}

void runTest( void (*test)(), const char* name )
{
    COCA_INFO( "==== starting test: " << name << " ====" );
    (*test)();
    COCA_INFO( "==== test finished: " << name << " ====" );
}

int main()
{
    COCA_INFO( "======== starting plugin test ========" );

    runTest( &myTest, "example test" );

    COCA_INFO( "======== plugin test finished ========" );

    return 0;
}

