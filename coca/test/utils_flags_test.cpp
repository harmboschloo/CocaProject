#include "utils_flags_test.h"
#include <coca/utils.h>

enum EFlag
{
    E_1 = (1<<0),
    E_2 = (1<<1),
    E_3 = (1<<2),
    E_4 = (1<<3),
};

void testFlags()
{
    coca::Flags<coca::uint8> flags;

    COCA_REQUIRE_EQUAL( flags.get(), 0 );
    COCA_REQUIRE_EQUAL( flags.isSet( E_1 ), false );
    COCA_REQUIRE_EQUAL( flags.isSet( E_2 ), false );
    COCA_REQUIRE_EQUAL( flags.isSet( E_3 ), false );
    COCA_REQUIRE_EQUAL( flags.isSet( E_4 ), false );
    COCA_REQUIRE_EQUAL( flags.areSet( E_2|E_4 ), false );
    flags.set( E_2|E_4 );
    COCA_REQUIRE_EQUAL( flags.isSet( E_1 ), false );
    COCA_REQUIRE_EQUAL( flags.isSet( E_2 ), true );
    COCA_REQUIRE_EQUAL( flags.isSet( E_3 ), false );
    COCA_REQUIRE_EQUAL( flags.isSet( E_4 ), true );
    COCA_REQUIRE_EQUAL( flags.areSet( E_2|E_4 ), true );
    COCA_REQUIRE_EQUAL( flags.areSet( E_1|E_4 ), false );
    flags.set( E_3, true );
    flags.set( E_2, false );
    COCA_REQUIRE_EQUAL( flags.isSet( E_1 ), false );
    COCA_REQUIRE_EQUAL( flags.isSet( E_2 ), false );
    COCA_REQUIRE_EQUAL( flags.isSet( E_3 ), true );
    COCA_REQUIRE_EQUAL( flags.isSet( E_4 ), true );
    COCA_REQUIRE_EQUAL( flags.areSet( E_2|E_4 ), false );
    COCA_REQUIRE_EQUAL( flags.areSet( E_3|E_4 ), true );
}
