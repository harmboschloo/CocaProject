#ifdef _MSC_VER
	#pragma warning (disable: 4127) // warning C4127: conditional expression is constant
#endif

#include "macro_test.h"

#include <coca/coca.h>

void testMacros()
{
    // test if all marcos compile, where posible
    COCA_INFO( "info message" );
    COCA_WARNING( "warning message" );
    COCA_ERROR( "error message" );
    COCA_DEBUG_INFO( "debug info message" );
    COCA_DEBUG_WARNING( "debug warning message" );
    COCA_DEBUG_ERROR( "debug error message" );
    COCA_ASSERT( true == true );
    COCA_ASSERT( false == false );
    COCA_ASSERT_EQUAL( true != true, true != true );
    COCA_ASSERT_EQUAL( true == true, true == true );
    COCA_ASSERT( true == true );
    COCA_CHECK( true == true );
    COCA_CHECK( true != true );
    COCA_CHECK_EQUAL( true != true, true != true );
    COCA_CHECK_EQUAL( true == true, true != true );
    COCA_CHECK_SILENT( true == true );
    COCA_CHECK_SILENT( true != true );
    COCA_CHECK_EQUAL_SILENT( true != true, true != true );
    COCA_CHECK_EQUAL_SILENT( true == true, true != true );
    COCA_REQUIRE( true == true );
    COCA_REQUIRE_EQUAL( true != true, true != true );
    COCA_REQUIRE_SILENT( true == true );
    COCA_REQUIRE_EQUAL_SILENT( true != true, true != true );
}
