#ifdef _MSC_VER
	#define NOMINMAX
	#pragma warning (disable: 4127) // warning C4127: conditional expression is constant
#endif

#include "utils_type_test.h"
#include <coca/utils.h>
#include <limits>

void testTypes()
{
    COCA_REQUIRE_EQUAL( sizeof( coca::bool8 ), (size_t)8/8 );
    COCA_REQUIRE_EQUAL( sizeof( coca::int8 ), (size_t)8/8 );
    COCA_REQUIRE_EQUAL( sizeof( coca::int16 ), (size_t)16/8 );
    COCA_REQUIRE_EQUAL( sizeof( coca::int32 ), (size_t)32/8 );
    COCA_REQUIRE_EQUAL( sizeof( coca::int64 ), (size_t)64/8 );
    COCA_REQUIRE_EQUAL( sizeof( coca::uint8 ), (size_t)8/8 );
    COCA_REQUIRE_EQUAL( sizeof( coca::uint16 ), (size_t)16/8 );
    COCA_REQUIRE_EQUAL( sizeof( coca::uint32 ), (size_t)32/8 );
    COCA_REQUIRE_EQUAL( sizeof( coca::uint64 ), (size_t)64/8 );
    COCA_REQUIRE_EQUAL( sizeof( coca::float32 ), (size_t)32/8 );
    COCA_REQUIRE_EQUAL( sizeof( coca::float64 ), (size_t)64/8 );

    COCA_REQUIRE_EQUAL( std::numeric_limits<coca::uint8>::min(), (size_t)0 );
    COCA_REQUIRE_EQUAL( std::numeric_limits<coca::uint16>::min(), (size_t)0 );
    COCA_REQUIRE_EQUAL( std::numeric_limits<coca::uint32>::min(), (size_t)0 );
    COCA_REQUIRE_EQUAL( std::numeric_limits<coca::uint64>::min(), (size_t)0 );
}
