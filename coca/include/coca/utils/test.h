// Copyright (C) 2008-2009 Harm Boschloo

#ifndef COCA_TEST_H_INCLUDED
#define COCA_TEST_H_INCLUDED

#include "debugBreak.h"
#include "../log/log.h"

namespace coca
{
    template<typename T>
    bool test( const T& value, bool pass = true, bool silent = true, bool required = false,
               const std::string& expression = "",
               const std::string& source = "", unsigned int line = 0 )
    {
        std::string passString(( pass ? "true" : "false" ) );
        bool boolValue = ( value != 0 );

        if ( boolValue != pass )
        {
            std::ostringstream  ss;
            if ( required ) { ss << "Required test"; }
            else { ss << "Test"; }
            ss << " failed! (" << expression << ") == (" << passString << ") : ("
            << value << ") != (" << pass << ")";
            LogMessage message( ss.str(), ( required ? E_ERROR_LOG_MESSAGE : E_WARNING_LOG_MESSAGE ),
                                source, line );
            getLogger().log( message );
            if ( required ) { debugBreak(); }
            return false;
        }
        else if ( !silent )
        {
            std::ostringstream  ss;
            if ( required ) { ss << "Required test"; }
            else { ss << "Test"; }
            ss << " passed! (" << expression << ") == (" << passString << ") : ("
            << value << ") == (" << pass << ")";
            LogMessage message( ss.str(), E_INFO_LOG_MESSAGE, source, line );
            getLogger().log( message );
        }

        return true;
    }

    template<typename T1, typename T2>
    bool test( const T1& value1, const T2& value2, bool pass = true, bool silent = true, bool required = false,
               const std::string& expression1 = "", const std::string& expression2 = "",
               const std::string& source = "", unsigned int line = 0 )
    {
        std::string passString(( pass ? "==" : "!=" ) );
        std::string failString(( pass ? "!=" : "==" ) );

        if ((( value1 ) == ( value2 ) ) != pass )
        {
            std::ostringstream  ss;
            if ( required ) { ss << "Required test"; }
            else { ss << "Test"; }
            ss << " failed! (" << expression1 << ") " << passString << " (" << expression2 << ") : ("
            << value1 << ") " << failString << " (" << value2 << ")";
            LogMessage message( ss.str(), ( required ? E_ERROR_LOG_MESSAGE : E_WARNING_LOG_MESSAGE ),
                                source, line );
            getLogger().log( message );
            if ( required ) { debugBreak(); }
            return false;
        }
        else if ( !silent )
        {
            std::ostringstream  ss;
            if ( required ) { ss << "Required test"; }
            else { ss << "Test"; }
            ss << " passed! (" << expression1 << ") " << passString << " (" << expression2 << ") : ("
            << value1 << ") " << passString << " (" << value2 << ")";
            LogMessage message( ss.str(), E_INFO_LOG_MESSAGE, source, line );
            getLogger().log( message );
        }

        return true;
    }

} // namespace coca

// CHECK //

#define COCA_CHECK( expresion ) coca::test( expresion, true, false, false, #expresion, __FILE__, __LINE__ )
#define COCA_CHECK_SILENT( expresion ) coca::test( expresion, true, true, false, #expresion, __FILE__, __LINE__ )
#define COCA_CHECK_EQUAL( expresion1, expresion2 ) coca::test( expresion1, expresion2, true, false, false, #expresion1, #expresion2, __FILE__, __LINE__ )
#define COCA_CHECK_EQUAL_SILENT( expresion1, expresion2 ) coca::test( expresion1, expresion2, true, true, false, #expresion1, #expresion2, __FILE__, __LINE__ )
#define COCA_CHECK_NOT_EQUAL( expresion1, expresion2 ) coca::test( expresion1, expresion2, false, false, false, #expresion1, #expresion2, __FILE__, __LINE__ )
#define COCA_CHECK_NOT_EQUAL_SILENT( expresion1, expresion2 ) coca::test( expresion1, expresion2, false, true, false, #expresion1, #expresion2, __FILE__, __LINE__ )
#define COCA_CHECK_NULL( expresion ) COCA_CHECK_EQUAL( expresion, (void*)0 )
#define COCA_CHECK_NULL_SILENT( expresion ) COCA_CHECK_EQUAL_SILENT( expresion, (void*)0 )
#define COCA_CHECK_NOT_NULL( expresion ) COCA_CHECK_NOT_EQUAL( expresion, (void*)0 )
#define COCA_CHECK_NOT_NULL_SILENT( expresion ) COCA_CHECK_NOT_EQUAL_SILENT( expresion, (void*)0 )

// REQUIRE //

#define COCA_REQUIRE( expresion ) { coca::test( expresion, true, false, true, #expresion, __FILE__, __LINE__ ); }
#define COCA_REQUIRE_SILENT( expresion ) { coca::test( expresion, true, true, true, #expresion, __FILE__, __LINE__ ); }
#define COCA_REQUIRE_EQUAL( expresion1, expresion2 ) { coca::test( expresion1, expresion2, true, false, true, #expresion1, #expresion2, __FILE__, __LINE__ ); }
#define COCA_REQUIRE_EQUAL_SILENT( expresion1, expresion2 ) { coca::test( expresion1, expresion2, true, true, true, #expresion1, #expresion2, __FILE__, __LINE__ ); }
#define COCA_REQUIRE_NOT_EQUAL( expresion1, expresion2 ) { coca::test( expresion1, expresion2, false, false, true, #expresion1, #expresion2, __FILE__, __LINE__ ); }
#define COCA_REQUIRE_NOT_EQUAL_SILENT( expresion1, expresion2 ) { coca::test( expresion1, expresion2, false, true, true, #expresion1, #expresion2, __FILE__, __LINE__ ); }
#define COCA_REQUIRE_NULL( expresion ) COCA_REQUIRE_EQUAL( expresion, (void*)0 )
#define COCA_REQUIRE_NULL_SILENT( expresion ) COCA_REQUIRE_EQUAL_SILENT( expresion, (void*)0 )
#define COCA_REQUIRE_NOT_NULL( expresion ) COCA_REQUIRE_NOT_EQUAL( expresion, (void*)0 )
#define COCA_REQUIRE_NOT_NULL_SILENT( expresion ) COCA_REQUIRE_NOT_EQUAL_SILENT( expresion, (void*)0 )

// ASSERT //

#ifdef NDEBUG
#define COCA_ASSERT( expression ) {}
#define COCA_ASSERT_EQUAL( expression1, expression2 ) {}
#else
#define COCA_ASSERT( expression ) COCA_REQUIRE_SILENT( expression )
#define COCA_ASSERT_EQUAL( expression1, expression2 ) COCA_REQUIRE_EQUAL_SILENT( expression1, expression2 )
#endif // NDEBUG


#endif // COCA_TEST_H_INCLUDED
