#include "utils_function_test.h"
#include <coca/utils.h>

class TestType : private coca::NonCopyable
{
public:
    TestType( const std::string& x = "" ) : value( x ) {}

    bool operator==( const TestType& other )
    {
        return value == other.value;
    }

    std::string value;
};

class FunctionTestClass : private coca::NonCopyable
{
public:
    FunctionTestClass() : intValue( 0 ), floatValue( 0 ), cbCount( 0 ) {}

    bool setInt( int a1 )
    {
        intValue = a1;
        return true;
    }

    bool setInt2( int a1 )
    {
        intValue = a1;
        return false;
    }

    bool setFloat( float a1 )
    {
        floatValue = a1;
        return false;
    }

    bool setFloat2( float a1 )
    {
        floatValue = a1;
        return true;
    }

    void add( int a1, float a2 )
    {
        intValue += a1;
        floatValue += a2;
    }

    void subtract( int a1, float a2 )
    {
        intValue -= a1;
        floatValue -= a2;
    }

    bool setTest( const TestType& test )
    {
        testValue.value = test.value;
        return true;
    }

    bool setTest2( const TestType& test )
    {
        testValue.value = test.value;
        return false;
    }

    const TestType& getTest()
    {
        return testValue;
    }

    const TestType& getTest2() const
    {
        return testValue;
    }

    void callback()
    {
        ++cbCount;
    }

    int intValue;
    float floatValue;
    TestType testValue;
    int cbCount;
};


class ConstFunctionTestClass : private coca::NonCopyable
{
public:
    ConstFunctionTestClass() : intValue( 0 ), floatValue( 0 ), cbCount( 0 ) {}

    bool setInt( const int& a1 )
    {
        intValue = a1;
        return true;
    }

    bool setInt2( const int& a1 ) const
    {
        intValue = a1;
        return false;
    }

    bool setFloat( const float a1 )
    {
        floatValue = a1;
        return false;
    }

    bool setFloat2( const float a1 ) const
    {
        floatValue = a1;
        return true;
    }

    void add( const int a1, const float a2 )
    {
        intValue += a1;
        floatValue += a2;
    }

    void subtract( const int a1, const float a2 ) const
    {
        intValue -= a1;
        floatValue -= a2;
    }

    const int& getInt() const
    {
        return intValue;
    }

    const float getFloat() const
    {
        return floatValue;
    }

    float getFloat2() const
    {
        return floatValue;
    }

    bool setTest( const TestType& test )
    {
        testValue.value = test.value;
        return true;
    }

    bool setTest2( const TestType& test ) const
    {
        testValue.value = test.value;
        return false;
    }

    const TestType& getTest()
    {
        return testValue;
    }

    const TestType& getTest2() const
    {
        return testValue;
    }

    void callback() const
    {
        ++cbCount;
    }

    mutable int intValue;
    mutable float floatValue;
    mutable TestType testValue;
    mutable int cbCount;
};


void testFunctions()
{
    // base tests
    {
        FunctionTestClass test;
        COCA_REQUIRE_EQUAL( test.intValue, 0 );
        COCA_REQUIRE_EQUAL( test.floatValue, 0 );
        COCA_REQUIRE_EQUAL( test.testValue.value, "" );

        coca::RefMemberFunction<bool,FunctionTestClass,int> setIntFunRef( &FunctionTestClass::setInt );
        COCA_REQUIRE_EQUAL( setIntFunRef( test, 123 ), true );
        COCA_REQUIRE_EQUAL( test.intValue, 123 );
        setIntFunRef = coca::makeRefMemberFunction( &FunctionTestClass::setInt2 );
        COCA_REQUIRE_EQUAL( setIntFunRef( test, 1234 ), false );
        COCA_REQUIRE_EQUAL( test.intValue, 1234 );

        coca::RefMemberFunction<bool,FunctionTestClass,const TestType&> setTestFunRef( &FunctionTestClass::setTest );
        TestType tt( "123" );
        COCA_REQUIRE_EQUAL( setTestFunRef( test, tt ), true );
        COCA_REQUIRE_EQUAL( test.testValue.value, "123" );

        coca::PtrMemberFunction<void,FunctionTestClass,int,float> addFunPtr( &FunctionTestClass::add );
        addFunPtr( &test, 5, 3.4f );
        COCA_REQUIRE_EQUAL( test.intValue, 1239 );
        COCA_REQUIRE_EQUAL( test.floatValue, 3.4f );
        addFunPtr = coca::makePtrMemberFunction( &FunctionTestClass::subtract );
        addFunPtr( &test, 5, 3.4f );
        COCA_REQUIRE_EQUAL( test.intValue, 1234 );
        COCA_REQUIRE_EQUAL( test.floatValue, 0 );

        coca::PtrMemberFunction<bool,FunctionTestClass,const TestType&> setTestFunPtr( &FunctionTestClass::setTest2 );
        tt.value = ( "321" );
        COCA_REQUIRE_EQUAL( setTestFunPtr( &test, tt ), false );
        COCA_REQUIRE_EQUAL( test.testValue.value, "321" );

        coca::ObjectFunction<bool,FunctionTestClass,float> setFloatFunObject( test, &FunctionTestClass::setFloat );
        COCA_REQUIRE_EQUAL( setFloatFunObject( 2.3f ), false );
        COCA_REQUIRE_EQUAL( test.floatValue, 2.3f );
        setFloatFunObject = coca::makeObjectFunction( test, &FunctionTestClass::setFloat2 );
        COCA_REQUIRE_EQUAL( setFloatFunObject( 2.36f ), true );
        COCA_REQUIRE_EQUAL( test.floatValue, 2.36f );

        coca::ObjectFunction<bool,FunctionTestClass,const TestType&> setTestFunObject( test, &FunctionTestClass::setTest );
        tt.value = ( "32123" );
        COCA_REQUIRE_EQUAL( setTestFunObject( tt ), true );
        COCA_REQUIRE_EQUAL( test.testValue.value, "32123" );
        setTestFunObject = coca::makeObjectFunction( test, &FunctionTestClass::setTest2 );
        tt.value = ( "4321234" );
        COCA_REQUIRE_EQUAL( setTestFunObject( tt ), false );
        COCA_REQUIRE_EQUAL( test.testValue.value, "4321234" );

        coca::ObjectFunctionAdaptor<bool,FunctionTestClass,float>* setFloatFunObjectAdaptor = coca::createObjectFunctionAdaptor( test, &FunctionTestClass::setFloat );
        delete setFloatFunObjectAdaptor;
        coca::ObjectFunctionAdaptor<bool,FunctionTestClass,const TestType&>* setTestFunObjectAdaptor = coca::createObjectFunctionAdaptor( test, &FunctionTestClass::setTest2 );
        delete setTestFunObjectAdaptor;
        coca::IFunctionAdaptor<>* cbAdaptor = coca::createObjectFunctionAdaptor( test, &FunctionTestClass::callback );
        COCA_REQUIRE_EQUAL( test.cbCount, 0 );
        ( *cbAdaptor )();
        COCA_REQUIRE_EQUAL( test.cbCount, 1 );
        delete cbAdaptor;

        ///@todo make several function calls work in msc
#ifndef COCA_MSC
        coca::Function<bool,float> setFloatFun( test, &FunctionTestClass::setFloat );
        COCA_REQUIRE_EQUAL( setFloatFun( 2.356f ), false );
        COCA_REQUIRE_EQUAL( test.floatValue, 2.356f );
        setFloatFun = coca::makeFunction( test, &FunctionTestClass::setFloat2 );
        COCA_REQUIRE_EQUAL( setFloatFun( 2.35f ), true );
        COCA_REQUIRE_EQUAL( test.floatValue, 2.35f );

        coca::Function<bool,const TestType&> setTestFun( test, &FunctionTestClass::setTest );
        tt.value = ( "bla" );
        COCA_REQUIRE_EQUAL( setTestFun( tt ), true );
        COCA_REQUIRE_EQUAL( test.testValue.value, "bla" );
        setTestFun = coca::makeFunction( test, &FunctionTestClass::setTest2 );
        tt.value = ( "bla2" );
        COCA_REQUIRE_EQUAL( setTestFun( tt ), false );
        COCA_REQUIRE_EQUAL( test.testValue.value, "bla2" );
        coca::Function<> cbFun( test, &FunctionTestClass::callback );
        cbFun();
        COCA_REQUIRE_EQUAL( test.cbCount, 2 );
#endif // not COCA_MSC

        coca::makeFunction<void>( test, &FunctionTestClass::getTest );
        coca::Function<void> getTestVoidFun( test, &FunctionTestClass::getTest );
    }

    // const tests
    {
        ConstFunctionTestClass test;
        COCA_REQUIRE_EQUAL( test.intValue, 0 );
        COCA_REQUIRE_EQUAL( test.floatValue, 0 );

        coca::RefMemberFunction<bool,ConstFunctionTestClass,const int&> setIntFunRef( &ConstFunctionTestClass::setInt );
        COCA_REQUIRE_EQUAL( setIntFunRef( test, 123 ), true );
        COCA_REQUIRE_EQUAL( test.intValue, 123 );
        coca::RefMemberFunction<bool,const ConstFunctionTestClass,const int&> setInt2FunRef = coca::makeRefMemberFunction( &ConstFunctionTestClass::setInt2 );
        COCA_REQUIRE_EQUAL( setInt2FunRef( test, 1234 ), false );
        COCA_REQUIRE_EQUAL( test.intValue, 1234 );

        coca::RefMemberFunction<bool,ConstFunctionTestClass,const TestType&> setTestFunRef( &ConstFunctionTestClass::setTest );
        TestType tt( "123" );
        COCA_REQUIRE_EQUAL( setTestFunRef( test, tt ), true );
        COCA_REQUIRE_EQUAL( test.testValue.value, "123" );

        coca::PtrMemberFunction<void,ConstFunctionTestClass,int,float> addFunPtr( &ConstFunctionTestClass::add );
        addFunPtr( &test, 5, 3.4f );
        COCA_REQUIRE_EQUAL( test.intValue, 1239 );
        COCA_REQUIRE_EQUAL( test.floatValue, 3.4f );
        coca::PtrMemberFunction<void,const ConstFunctionTestClass,int,float> subtractFunPtr = coca::makePtrMemberFunction( &ConstFunctionTestClass::subtract );
        subtractFunPtr( &test, 5, 3.4f );
        COCA_REQUIRE_EQUAL( test.intValue, 1234 );
        COCA_REQUIRE_EQUAL( test.floatValue, 0 );

        coca::PtrMemberFunction<bool,const ConstFunctionTestClass,const TestType&> setTestFunPtr( &ConstFunctionTestClass::setTest2 );
        tt.value = ( "321" );
        COCA_REQUIRE_EQUAL( setTestFunPtr( &test, tt ), false );
        COCA_REQUIRE_EQUAL( test.testValue.value, "321" );

        coca::ObjectFunction<bool,ConstFunctionTestClass,float> setFloatFunObject( test, &ConstFunctionTestClass::setFloat );
        COCA_REQUIRE_EQUAL( setFloatFunObject( 2.3f ), false );
        COCA_REQUIRE_EQUAL( test.floatValue, 2.3f );
        coca::ObjectFunction<bool,const ConstFunctionTestClass,float> setFloat2FunObject = coca::makeObjectFunction( test, &ConstFunctionTestClass::setFloat2 );
        COCA_REQUIRE_EQUAL( setFloat2FunObject( 2.36f ), true );
        COCA_REQUIRE_EQUAL( test.floatValue, 2.36f );

        coca::ObjectFunction<bool,ConstFunctionTestClass,const TestType&> setTestFunObject( test, &ConstFunctionTestClass::setTest );
        tt.value = ( "32123" );
        COCA_REQUIRE_EQUAL( setTestFunObject( tt ), true );
        COCA_REQUIRE_EQUAL( test.testValue.value, "32123" );
        coca::ObjectFunction<bool,const ConstFunctionTestClass,const TestType&> setTest2FunObject = coca::makeObjectFunction( test, &ConstFunctionTestClass::setTest2 );
        tt.value = ( "4321234" );
        COCA_REQUIRE_EQUAL( setTest2FunObject( tt ), false );
        COCA_REQUIRE_EQUAL( test.testValue.value, "4321234" );

        coca::ObjectFunctionAdaptor<bool,ConstFunctionTestClass,float>* setFloatFunObjectAdaptor = coca::createObjectFunctionAdaptor( test, &ConstFunctionTestClass::setFloat );
        delete setFloatFunObjectAdaptor;
        coca::IFunctionAdaptor<>* cbAdaptor = coca::createObjectFunctionAdaptor( test, &ConstFunctionTestClass::callback );
        COCA_REQUIRE_EQUAL( test.cbCount, 0 );
        ( *cbAdaptor )();
        COCA_REQUIRE_EQUAL( test.cbCount, 1 );
        delete cbAdaptor;

        ///@todo make several function calls work in msc
#ifndef COCA_MSC
        coca::Function<bool,float> setFloatFun( test, &ConstFunctionTestClass::setFloat );
        COCA_REQUIRE_EQUAL( setFloatFun( 2.356f ), false );
        COCA_REQUIRE_EQUAL( test.floatValue, 2.356f );

        setFloatFun = coca::makeFunction( test, &ConstFunctionTestClass::setFloat2 );
        COCA_REQUIRE_EQUAL( setFloatFun( 6.35f ), true );
        COCA_REQUIRE_EQUAL( test.floatValue, 6.35f );

        coca::Function<bool,const TestType&> setTestFun( test, &ConstFunctionTestClass::setTest );
        tt.value = ( "bla" );
        COCA_REQUIRE_EQUAL( setTestFun( tt ), true );
        COCA_REQUIRE_EQUAL( test.testValue.value, "bla" );
        setTestFun = coca::makeFunction( test, &ConstFunctionTestClass::setTest2 );
        tt.value = ( "bla2" );
        COCA_REQUIRE_EQUAL( setTestFun( tt ), false );
        COCA_REQUIRE_EQUAL( test.testValue.value, "bla2" );

        coca::Function<const int&> getIntFun( test, &ConstFunctionTestClass::getInt );
        COCA_REQUIRE_EQUAL( getIntFun(), 1234 );
        getIntFun = coca::makeFunction( test, &ConstFunctionTestClass::getInt );
        COCA_REQUIRE_EQUAL( getIntFun(), 1234 );

        coca::Function<const float> getFloatFun( test, &ConstFunctionTestClass::getFloat );
        COCA_REQUIRE_EQUAL( getFloatFun(), 6.35f );
        getFloatFun = coca::makeFunction( test, &ConstFunctionTestClass::getFloat );
        COCA_REQUIRE_EQUAL( getFloatFun(), 6.35f );
        coca::Function<> cbFun( test, &ConstFunctionTestClass::callback );
        cbFun();
        COCA_REQUIRE_EQUAL( test.cbCount, 2 );
#endif // not COCA_MSC

        /// @todo: get this working:
        ///coca::makeFunction<double>( test, &ConstFunctionTestClass::getFloat2 );
        ///coca::Function<double> getFloatDoubleFun( test, &ConstFunctionTestClass::getFloat );
        coca::Function<double> funtest( coca::createObjectFunctionAdaptor<double>( test, &ConstFunctionTestClass::getFloat ) );

    }
}
