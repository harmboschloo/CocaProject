#include "macro_test.h"
//#include "utils_singleton_test.h"
#include "utils_string_test.h"
#include "utils_type_test.h"
#include "utils_function_test.h"
#include "utils_accessor_test.h"
#include "utils_flags_test.h"
#include "utils_math_test.h"
#include "attribute_test.h"
#include "component_node_test.h"
#include "factory_test.h"
#include <coca/coca.h>

coca::FileLogMessageHandler _fileLog( "coca_test.log" );
coca::StdLogMessageHandler _stdLog;

void runTest( void (*test)(), const char* name )
{
    COCA_INFO( "==== starting test: " << name << " ====" );
    (*test)();
    COCA_INFO( "==== test finished: " << name << " ====" );
}

int main()
{
    COCA_INFO( "======== starting coca test ========" );

    runTest( &testMacros, "macro test" );
    //runTest( &testSingleton, "singleton test" );
    runTest( &testStringOperations, "string operation test" );
    runTest( &testTypes, "type test" );
    runTest( &testFunctions, "function test" );
    runTest( &testAccessors, "accessor test" );
    runTest( &testFlags, "flags test" );
    runTest( &testMath, "math test" );
    runTest( &testAttributes, "attribute test" );
    runTest( &testComponentNode, "component node test" );
    runTest( &testFactory, "factory test" );

    COCA_INFO( "======== coca test finished ========" );

    return 0;
}
