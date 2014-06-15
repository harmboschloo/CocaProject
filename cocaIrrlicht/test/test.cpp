#include <coca/coca.h>

coca::StdLogMessageHandler _stdLog;
coca::FileLogMessageHandler _fileLog( "test.log" );

void runTest( void (*test)(), const char* name )
{
    COCA_INFO( "==== starting test: " << name << " ====" );
    (*test)();
    COCA_INFO( "==== test finished: " << name << " ====" );
}

int main()
{
    COCA_INFO( "======== starting plugin test ========" );


    COCA_INFO( "======== plugin test finished ========" );

    return 0;
}

