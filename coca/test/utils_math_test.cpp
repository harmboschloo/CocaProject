#include "utils_math_test.h"
#include <coca/utils/math.h>
#include <coca/utils/test.h>

void testMath()
{
    COCA_REQUIRE_EQUAL( coca::square( 3 ), 9 );
    COCA_REQUIRE_EQUAL( coca::square( -3 ), 9 );
    COCA_REQUIRE_EQUAL( coca::cubic( 3 ), 27 );
    COCA_REQUIRE_EQUAL( coca::cubic( -3 ), -27 );
    COCA_REQUIRE_EQUAL( coca::sign( -3 ), -1 );
    COCA_REQUIRE_EQUAL( coca::sign( 0 ), 0 );
    COCA_REQUIRE_EQUAL( coca::sign( 0.123 ), 1 );
    COCA_REQUIRE_EQUAL( coca::limit( 0.123, 0.0, 0.12 ), 0.12 );
    COCA_REQUIRE_EQUAL( coca::limit( -0.123f, -1.f, 0.12f ), -0.123f );
    COCA_REQUIRE_EQUAL( coca::limit( 123, 200, 300 ), 200 );
    COCA_REQUIRE_EQUAL( coca::max( 123, 200 ), 200 );
    COCA_REQUIRE_EQUAL( coca::max( -123.34, -200.4 ), -123.34 );
    COCA_REQUIRE_EQUAL( coca::min( -123.34, -200.4 ), -200.4 );
    COCA_REQUIRE_EQUAL( coca::abs( -123.34 ), 123.34 );
    COCA_REQUIRE_EQUAL( coca::abs( 135 ),135 );
    COCA_REQUIRE_EQUAL( coca::limitAngleDegrees( 0 ),0 );
    COCA_REQUIRE_EQUAL( coca::limitAngleDegrees( 135 ),135 );
    COCA_REQUIRE_EQUAL( coca::limitAngleDegrees( 360 ),0 );
    COCA_REQUIRE_EQUAL( coca::limitAngleDegrees( 635 ),275 );
    COCA_REQUIRE_EQUAL( coca::limitAngleDegrees( -635 ),85 );
    COCA_REQUIRE_EQUAL( coca::limitAngleDegrees( 0.0 ),0.0 );
    COCA_REQUIRE_EQUAL( coca::limitAngleDegrees( 135.56 ),135.56 );
    COCA_REQUIRE_EQUAL( coca::limitAngleDegrees( 360.01 ),(360.01-360) );
    COCA_REQUIRE_EQUAL( coca::limitAngleDegrees( 635.123 ),(635.123-360) );
    COCA_REQUIRE_EQUAL( coca::limitAngleDegrees( -635.43 ),(-635.43+2*360) );
    COCA_REQUIRE_EQUAL( coca::limitAngleDegrees( 0.0f, -180.f ),0.0f );
    COCA_REQUIRE_EQUAL( coca::limitAngleDegrees( 195.56f, -180.f ),(195.56f-360) );
    COCA_REQUIRE_EQUAL( coca::limitAngleDegrees( 360.01f, -180.f ),(360.01f-360) );
    COCA_REQUIRE_EQUAL( coca::limitAngleDegrees( 635.123f, -180.f ),(635.123f-2*360) );
    COCA_REQUIRE_EQUAL( coca::limitAngleDegrees( -635.43f, -180.f ),(-635.43f+2*360) );
    COCA_REQUIRE_EQUAL( coca::limitAngleRadians( 0.0f ),0.0f );
    COCA_REQUIRE_EQUAL( coca::limitAngleRadians( 2*coca::PI ),(2*coca::PI-2*coca::PI) );
    COCA_REQUIRE_EQUAL( coca::limitAngleRadians( 3*coca::PI ),(3*coca::PI-2*coca::PI) );
}
