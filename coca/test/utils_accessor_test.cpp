#include "utils_accessor_test.h"
#include <coca/utils.h>

class ATestType : private coca::NonCopyable
{
public:
    ATestType( const std::string& x = "" ) : value( x ) {}

    bool operator==( const ATestType& other )
    {
        return value == other.value;
    }

    std::string value;
};

class AccessorTestClass : private coca::NonCopyable
{
public:
    AccessorTestClass() : intValue( 0 ), floatValue( 0 ) {}

    int getInt()
    {
        return intValue;
    }

    int getIntConst() const
    {
        return intValue;
    }

    void setInt( int a1 )
    {
        intValue = a1;
    }

    void setIntConst( int a1 ) const
    {
        intValue = a1;
    }

    bool setIntReturn( int a1 )
    {
        intValue = a1;
        return true;
    }

    bool setIntConstReturn( int a1 ) const
    {
        intValue = a1;
        return false;
    }


    void setTest( const ATestType& test )
    {
        testValue.value = test.value;
    }

    const ATestType& getTest() const
    {
        return testValue;
    }

    mutable int intValue;
    float floatValue;
    ATestType testValue;
};

void testRefMemberFunctionAccessor()
{
    AccessorTestClass test;
    COCA_REQUIRE_EQUAL( test.intValue, 0 );
    COCA_REQUIRE_EQUAL( test.floatValue, 0 );
    COCA_REQUIRE_EQUAL( test.testValue.value, "" );

    coca::RefMemberFunctionAccessor<int,AccessorTestClass> accIntFunRef( &AccessorTestClass::getInt, &AccessorTestClass::setInt );
    accIntFunRef( test, 123 );
    COCA_REQUIRE_EQUAL( test.intValue, 123 );
    COCA_REQUIRE_EQUAL( accIntFunRef( test ), 123 );
    accIntFunRef = coca::makeRefMemberAccessor( &AccessorTestClass::getInt, &AccessorTestClass::setInt );
    accIntFunRef( test, 6123 );
    COCA_REQUIRE_EQUAL( test.intValue, 6123 );
    COCA_REQUIRE_EQUAL( accIntFunRef( test ), 6123 );

    coca::RefMemberFunctionAccessor<int,const AccessorTestClass,void,AccessorTestClass,int> accIntFunRef2( &AccessorTestClass::getIntConst, &AccessorTestClass::setInt );
    accIntFunRef2( test, 1234 );
    COCA_REQUIRE_EQUAL( test.intValue, 1234 );
    COCA_REQUIRE_EQUAL( accIntFunRef2( test ), 1234 );
    accIntFunRef2 = coca::makeRefMemberAccessor( &AccessorTestClass::getIntConst, &AccessorTestClass::setInt );
    accIntFunRef2( test, 81234 );
    COCA_REQUIRE_EQUAL( test.intValue, 81234 );
    COCA_REQUIRE_EQUAL( accIntFunRef2( test ), 81234 );

    coca::RefMemberFunctionAccessor<int,AccessorTestClass,void,const AccessorTestClass,int> accIntFunRef3( &AccessorTestClass::getInt, &AccessorTestClass::setIntConst );
    accIntFunRef3( test, 12345 );
    COCA_REQUIRE_EQUAL( test.intValue, 12345 );
    COCA_REQUIRE_EQUAL( accIntFunRef3( test ), 12345 );
    accIntFunRef3 = coca::makeRefMemberAccessor( &AccessorTestClass::getInt, &AccessorTestClass::setIntConst );
    accIntFunRef3( test, 512345 );
    COCA_REQUIRE_EQUAL( test.intValue, 512345 );
    COCA_REQUIRE_EQUAL( accIntFunRef3( test ), 512345 );

    coca::RefMemberFunctionAccessor<int,const AccessorTestClass> accIntFunRef4( &AccessorTestClass::getIntConst, &AccessorTestClass::setIntConst );
    accIntFunRef4( test, 123456 );
    COCA_REQUIRE_EQUAL( test.intValue, 123456 );
    COCA_REQUIRE_EQUAL( accIntFunRef4( test ), 123456 );
    accIntFunRef4 = coca::makeRefMemberAccessor( &AccessorTestClass::getIntConst, &AccessorTestClass::setIntConst );
    accIntFunRef4( test, 1323456 );
    COCA_REQUIRE_EQUAL( test.intValue, 1323456 );
    COCA_REQUIRE_EQUAL( accIntFunRef4( test ), 1323456 );

    coca::RefMemberFunctionAccessor<int,AccessorTestClass,bool> accIntFunRef5( &AccessorTestClass::getInt, &AccessorTestClass::setIntReturn );
    COCA_REQUIRE_EQUAL( accIntFunRef5( test, 13 ), true );
    COCA_REQUIRE_EQUAL( test.intValue, 13 );
    COCA_REQUIRE_EQUAL( accIntFunRef5( test ), 13 );
    accIntFunRef5 = coca::makeRefMemberAccessor( &AccessorTestClass::getInt, &AccessorTestClass::setIntReturn );
    COCA_REQUIRE_EQUAL( accIntFunRef5( test, 213 ), true );
    COCA_REQUIRE_EQUAL( test.intValue, 213 );
    COCA_REQUIRE_EQUAL( accIntFunRef5( test ), 213 );

    coca::RefMemberFunctionAccessor<int,AccessorTestClass,bool,const AccessorTestClass,int> accIntFunRef6( &AccessorTestClass::getInt, &AccessorTestClass::setIntConstReturn );
    COCA_REQUIRE_EQUAL( accIntFunRef6( test, 18 ), false );
    COCA_REQUIRE_EQUAL( test.intValue, 18 );
    COCA_REQUIRE_EQUAL( accIntFunRef6( test ), 18 );
    accIntFunRef6 = coca::makeRefMemberAccessor( &AccessorTestClass::getInt, &AccessorTestClass::setIntConstReturn );
    COCA_REQUIRE_EQUAL( accIntFunRef6( test, 618 ), false );
    COCA_REQUIRE_EQUAL( test.intValue, 618 );
    COCA_REQUIRE_EQUAL( accIntFunRef6( test ), 618 );

    ATestType tt;
    COCA_REQUIRE_EQUAL( tt.value, "" );
    COCA_REQUIRE_EQUAL( test.testValue.value, "" );
    coca::RefMemberFunctionAccessor<const ATestType&,const AccessorTestClass,void,AccessorTestClass> accTestFunRef = coca::makeRefMemberAccessor( &AccessorTestClass::getTest, &AccessorTestClass::setTest );
    tt.value = "bla";
    accTestFunRef( test, tt );
    COCA_REQUIRE_EQUAL( test.testValue.value, "bla" );
    COCA_REQUIRE_EQUAL( accTestFunRef( test ).value, "bla" );
}

void testPtrMemberFunctionAccessor()
{
    AccessorTestClass test;
    COCA_REQUIRE_EQUAL( test.intValue, 0 );
    COCA_REQUIRE_EQUAL( test.floatValue, 0 );
    COCA_REQUIRE_EQUAL( test.testValue.value, "" );

    coca::PtrMemberFunctionAccessor<int,AccessorTestClass> accIntFunPtr( &AccessorTestClass::getInt, &AccessorTestClass::setInt );
    accIntFunPtr( &test, 123 );
    COCA_REQUIRE_EQUAL( test.intValue, 123 );
    COCA_REQUIRE_EQUAL( accIntFunPtr( &test ), 123 );
    accIntFunPtr = coca::makePtrMemberAccessor( &AccessorTestClass::getInt, &AccessorTestClass::setInt );
    accIntFunPtr( &test, 6123 );
    COCA_REQUIRE_EQUAL( test.intValue, 6123 );
    COCA_REQUIRE_EQUAL( accIntFunPtr( &test ), 6123 );

    coca::PtrMemberFunctionAccessor<int,const AccessorTestClass,void,AccessorTestClass,int> accIntFunPtr2( &AccessorTestClass::getIntConst, &AccessorTestClass::setInt );
    accIntFunPtr2( &test, 1234 );
    COCA_REQUIRE_EQUAL( test.intValue, 1234 );
    COCA_REQUIRE_EQUAL( accIntFunPtr2( &test ), 1234 );
    accIntFunPtr2 = coca::makePtrMemberAccessor( &AccessorTestClass::getIntConst, &AccessorTestClass::setInt );
    accIntFunPtr2( &test, 81234 );
    COCA_REQUIRE_EQUAL( test.intValue, 81234 );
    COCA_REQUIRE_EQUAL( accIntFunPtr2( &test ), 81234 );

    coca::PtrMemberFunctionAccessor<int,AccessorTestClass,void,const AccessorTestClass,int> accIntFunPtr3( &AccessorTestClass::getInt, &AccessorTestClass::setIntConst );
    accIntFunPtr3( &test, 12345 );
    COCA_REQUIRE_EQUAL( test.intValue, 12345 );
    COCA_REQUIRE_EQUAL( accIntFunPtr3( &test ), 12345 );
    accIntFunPtr3 = coca::makePtrMemberAccessor( &AccessorTestClass::getInt, &AccessorTestClass::setIntConst );
    accIntFunPtr3( &test, 512345 );
    COCA_REQUIRE_EQUAL( test.intValue, 512345 );
    COCA_REQUIRE_EQUAL( accIntFunPtr3( &test ), 512345 );

    coca::PtrMemberFunctionAccessor<int,const AccessorTestClass> accIntFunPtr4( &AccessorTestClass::getIntConst, &AccessorTestClass::setIntConst );
    accIntFunPtr4( &test, 123456 );
    COCA_REQUIRE_EQUAL( test.intValue, 123456 );
    COCA_REQUIRE_EQUAL( accIntFunPtr4( &test ), 123456 );
    accIntFunPtr4 = coca::makePtrMemberAccessor( &AccessorTestClass::getIntConst, &AccessorTestClass::setIntConst );
    accIntFunPtr4( &test, 1323456 );
    COCA_REQUIRE_EQUAL( test.intValue, 1323456 );
    COCA_REQUIRE_EQUAL( accIntFunPtr4( &test ), 1323456 );

    coca::PtrMemberFunctionAccessor<int,AccessorTestClass,bool> accIntFunPtr5( &AccessorTestClass::getInt, &AccessorTestClass::setIntReturn );
    COCA_REQUIRE_EQUAL( accIntFunPtr5( &test, 13 ), true );
    COCA_REQUIRE_EQUAL( test.intValue, 13 );
    COCA_REQUIRE_EQUAL( accIntFunPtr5( &test ), 13 );
    accIntFunPtr5 = coca::makePtrMemberAccessor( &AccessorTestClass::getInt, &AccessorTestClass::setIntReturn );
    COCA_REQUIRE_EQUAL( accIntFunPtr5( &test, 213 ), true );
    COCA_REQUIRE_EQUAL( test.intValue, 213 );
    COCA_REQUIRE_EQUAL( accIntFunPtr5( &test ), 213 );

    coca::PtrMemberFunctionAccessor<int,AccessorTestClass,bool,const AccessorTestClass,int> accIntFunPtr6( &AccessorTestClass::getInt, &AccessorTestClass::setIntConstReturn );
    COCA_REQUIRE_EQUAL( accIntFunPtr6( &test, 18 ), false );
    COCA_REQUIRE_EQUAL( test.intValue, 18 );
    COCA_REQUIRE_EQUAL( accIntFunPtr6( &test ), 18 );
    accIntFunPtr6 = coca::makePtrMemberAccessor( &AccessorTestClass::getInt, &AccessorTestClass::setIntConstReturn );
    COCA_REQUIRE_EQUAL( accIntFunPtr6( &test, 618 ), false );
    COCA_REQUIRE_EQUAL( test.intValue, 618 );
    COCA_REQUIRE_EQUAL( accIntFunPtr6( &test ), 618 );

    ATestType tt;
    COCA_REQUIRE_EQUAL( tt.value, "" );
    COCA_REQUIRE_EQUAL( test.testValue.value, "" );
    coca::PtrMemberFunctionAccessor<const ATestType&,const AccessorTestClass,void,AccessorTestClass> accTestFunPtr = coca::makePtrMemberAccessor( &AccessorTestClass::getTest, &AccessorTestClass::setTest );
    tt.value = "bla";
    accTestFunPtr( &test, tt );
    COCA_REQUIRE_EQUAL( test.testValue.value, "bla" );
    COCA_REQUIRE_EQUAL( accTestFunPtr( &test ).value, "bla" );
}

void testRefMemberDataAccessor()
{
    AccessorTestClass test;
    COCA_REQUIRE_EQUAL( test.intValue, 0 );
    COCA_REQUIRE_EQUAL( test.floatValue, 0 );
    COCA_REQUIRE_EQUAL( test.testValue.value, "" );

    coca::RefMemberDataAccessor<int,AccessorTestClass> accIntRef( &AccessorTestClass::intValue );
    accIntRef( test, 123 );
    COCA_REQUIRE_EQUAL( test.intValue, 123 );
    COCA_REQUIRE_EQUAL( accIntRef( test ), 123 );
}

void testPtrMemberDataAccessor()
{
    AccessorTestClass test;
    COCA_REQUIRE_EQUAL( test.intValue, 0 );
    COCA_REQUIRE_EQUAL( test.floatValue, 0 );
    COCA_REQUIRE_EQUAL( test.testValue.value, "" );

    coca::PtrMemberDataAccessor<int,AccessorTestClass> accIntPtr( &AccessorTestClass::intValue );
    accIntPtr( &test, 123 );
    COCA_REQUIRE_EQUAL( test.intValue, 123 );
    COCA_REQUIRE_EQUAL( accIntPtr( &test ), 123 );
}

void testObjectFunctionAccessor()
{
    AccessorTestClass test;
    COCA_REQUIRE_EQUAL( test.intValue, 0 );
    COCA_REQUIRE_EQUAL( test.floatValue, 0 );
    COCA_REQUIRE_EQUAL( test.testValue.value, "" );

    coca::ObjectFunctionAccessor<AccessorTestClass,int,AccessorTestClass,void,AccessorTestClass,int> accInt =
        coca::makeObjectAccessor( test, &AccessorTestClass::getInt, &AccessorTestClass::setInt );
    accInt( 123 );
    COCA_REQUIRE_EQUAL( test.intValue, 123 );
    COCA_REQUIRE_EQUAL( accInt(), 123 );

    coca::ObjectFunctionAccessor<AccessorTestClass,int,const AccessorTestClass,void,AccessorTestClass,int> accInt2 =
        coca::makeObjectAccessor( test, &AccessorTestClass::getIntConst, &AccessorTestClass::setInt );
    accInt2( 1234 );
    COCA_REQUIRE_EQUAL( test.intValue, 1234 );
    COCA_REQUIRE_EQUAL( accInt2(), 1234 );

    coca::ObjectFunctionAccessor<AccessorTestClass,int,AccessorTestClass,void,const AccessorTestClass,int> accInt3 =
        coca::makeObjectAccessor( test, &AccessorTestClass::getInt, &AccessorTestClass::setIntConst );
    accInt3( 12345 );
    COCA_REQUIRE_EQUAL( test.intValue, 12345 );
    COCA_REQUIRE_EQUAL( accInt3(), 12345 );

    coca::ObjectFunctionAccessor<const AccessorTestClass,int,const AccessorTestClass,void,const AccessorTestClass,int> accInt4 =
        coca::makeObjectAccessor( test, &AccessorTestClass::getIntConst, &AccessorTestClass::setIntConst );
    accInt4( 123456 );
    COCA_REQUIRE_EQUAL( test.intValue, 123456 );
    COCA_REQUIRE_EQUAL( accInt4(), 123456 );

    coca::ObjectFunctionAccessor<AccessorTestClass,int,const AccessorTestClass,bool,AccessorTestClass,int> accInt5 =
        coca::makeObjectAccessor( test, &AccessorTestClass::getIntConst, &AccessorTestClass::setIntReturn );
    COCA_REQUIRE_EQUAL( accInt5( 2123456 ), true );
    COCA_REQUIRE_EQUAL( test.intValue, 2123456 );
    COCA_REQUIRE_EQUAL( accInt5(), 2123456 );

    coca::ObjectFunctionAccessor<const AccessorTestClass,int,const AccessorTestClass,bool,const AccessorTestClass,int> accInt6 =
        coca::makeObjectAccessor( test, &AccessorTestClass::getIntConst, &AccessorTestClass::setIntConstReturn );
    COCA_REQUIRE_EQUAL( accInt6( 2126 ), false );
    COCA_REQUIRE_EQUAL( test.intValue, 2126 );
    COCA_REQUIRE_EQUAL( accInt6(), 2126 );

    ATestType tt;
    COCA_REQUIRE_EQUAL( tt.value, "" );
    COCA_REQUIRE_EQUAL( test.testValue.value, "" );
    coca::ObjectFunctionAccessor<AccessorTestClass,const ATestType&> accInt7 =
        coca::makeObjectAccessor( test, &AccessorTestClass::getTest, &AccessorTestClass::setTest );
    tt.value = "bla";
    accInt7( tt );
    COCA_REQUIRE_EQUAL( test.testValue.value, "bla" );
    COCA_REQUIRE_EQUAL( accInt7().value, "bla" );
}

void testObjectFunctionAccessorAdaptor()
{
    AccessorTestClass test;
    COCA_REQUIRE_EQUAL( test.intValue, 0 );
    COCA_REQUIRE_EQUAL( test.floatValue, 0 );
    COCA_REQUIRE_EQUAL( test.testValue.value, "" );

    coca::ObjectFunctionAccessorAdaptor<AccessorTestClass,int,AccessorTestClass,void,AccessorTestClass,int>* accInt =
        coca::createAccessorAdaptor( test, &AccessorTestClass::getInt, &AccessorTestClass::setInt );
    (*accInt)( 123 );
    COCA_REQUIRE_EQUAL( test.intValue, 123 );
    COCA_REQUIRE_EQUAL( (*accInt)(), 123 );
    delete accInt;

    coca::ObjectFunctionAccessorAdaptor<AccessorTestClass,int,const AccessorTestClass,void,AccessorTestClass,int>* accInt2 =
        coca::createAccessorAdaptor( test, &AccessorTestClass::getIntConst, &AccessorTestClass::setInt );
    (*accInt2)( 1234 );
    COCA_REQUIRE_EQUAL( test.intValue, 1234 );
    COCA_REQUIRE_EQUAL( (*accInt2)(), 1234 );
    delete accInt2;

    coca::ObjectFunctionAccessorAdaptor<AccessorTestClass,int,AccessorTestClass,void,const AccessorTestClass,int>* accInt3 =
        coca::createAccessorAdaptor( test, &AccessorTestClass::getInt, &AccessorTestClass::setIntConst );
    (*accInt3)( 12345 );
    COCA_REQUIRE_EQUAL( test.intValue, 12345 );
    COCA_REQUIRE_EQUAL( (*accInt3)(), 12345 );
    delete accInt3;

    coca::ObjectFunctionAccessorAdaptor<const AccessorTestClass,int,const AccessorTestClass,void,const AccessorTestClass,int>* accInt4 =
        coca::createAccessorAdaptor( test, &AccessorTestClass::getIntConst, &AccessorTestClass::setIntConst );
    (*accInt4)( 123456 );
    COCA_REQUIRE_EQUAL( test.intValue, 123456 );
    COCA_REQUIRE_EQUAL( (*accInt4)(), 123456 );
    delete accInt4;

    coca::ObjectFunctionAccessorAdaptor<AccessorTestClass,int,const AccessorTestClass,bool,AccessorTestClass,int>* accInt5 =
        coca::createAccessorAdaptor( test, &AccessorTestClass::getIntConst, &AccessorTestClass::setIntReturn );
    COCA_REQUIRE_EQUAL( (*accInt5)( 2123456 ), true );
    COCA_REQUIRE_EQUAL( test.intValue, 2123456 );
    COCA_REQUIRE_EQUAL( (*accInt5)(), 2123456 );
    delete accInt5;

    coca::ObjectFunctionAccessorAdaptor<const AccessorTestClass,int,const AccessorTestClass,bool,const AccessorTestClass,int>* accInt6 =
        coca::createAccessorAdaptor( test, &AccessorTestClass::getIntConst, &AccessorTestClass::setIntConstReturn );
    COCA_REQUIRE_EQUAL( (*accInt6)( 2126 ), false );
    COCA_REQUIRE_EQUAL( test.intValue, 2126 );
    COCA_REQUIRE_EQUAL( (*accInt6)(), 2126 );
    delete accInt6;

    ATestType tt;
    COCA_REQUIRE_EQUAL( tt.value, "" );
    COCA_REQUIRE_EQUAL( test.testValue.value, "" );
    coca::ObjectFunctionAccessorAdaptor<AccessorTestClass,const ATestType&>* accInt7 =
        coca::createAccessorAdaptor( test, &AccessorTestClass::getTest, &AccessorTestClass::setTest );
    tt.value = "bla";
    (*accInt7)( tt );
    COCA_REQUIRE_EQUAL( test.testValue.value, "bla" );
    COCA_REQUIRE_EQUAL( (*accInt7)().value, "bla" );
    delete accInt7;
}

void testObjectDataAccessor()
{
    AccessorTestClass test;
    COCA_REQUIRE_EQUAL( test.intValue, 0 );
    COCA_REQUIRE_EQUAL( test.floatValue, 0 );
    COCA_REQUIRE_EQUAL( test.testValue.value, "" );

    coca::ObjectDataAccessor<int,AccessorTestClass> accInt = coca::makeObjectAccessor( test, &AccessorTestClass::intValue );
    accInt( 123 );
    COCA_REQUIRE_EQUAL( test.intValue, 123 );
    COCA_REQUIRE_EQUAL( accInt(), 123 );
}

void testObjectDataAccessorAdaptor()
{
    AccessorTestClass test;
    COCA_REQUIRE_EQUAL( test.intValue, 0 );
    COCA_REQUIRE_EQUAL( test.floatValue, 0 );
    COCA_REQUIRE_EQUAL( test.testValue.value, "" );

    coca::ObjectDataAccessorAdaptor<int,AccessorTestClass>* accInt = coca::createAccessorAdaptor( test, &AccessorTestClass::intValue );
    (*accInt)( 1323 );
    COCA_REQUIRE_EQUAL( test.intValue, 1323 );
    COCA_REQUIRE_EQUAL( (*accInt)(), 1323 );
    delete accInt;
}

void testDataAccessor()
{
    AccessorTestClass test;
    COCA_REQUIRE_EQUAL( test.intValue, 0 );
    COCA_REQUIRE_EQUAL( test.floatValue, 0 );
    COCA_REQUIRE_EQUAL( test.testValue.value, "" );

    coca::DataAccessor<int> accInt = coca::makeDataAccessor( test.intValue );
    accInt( 123 );
    COCA_REQUIRE_EQUAL( test.intValue, 123 );
    COCA_REQUIRE_EQUAL( accInt(), 123 );
}

void testDataAccessorAdaptor()
{
    AccessorTestClass test;
    COCA_REQUIRE_EQUAL( test.intValue, 0 );
    COCA_REQUIRE_EQUAL( test.floatValue, 0 );
    COCA_REQUIRE_EQUAL( test.testValue.value, "" );

    coca::DataAccessorAdaptor<int>* accInt = coca::createAccessorAdaptor( test.intValue );
    (*accInt)( 1423 );
    COCA_REQUIRE_EQUAL( test.intValue, 1423 );
    COCA_REQUIRE_EQUAL( (*accInt)(), 1423 );
    delete accInt;
}

void testAccessor()
{
    AccessorTestClass test;
    COCA_REQUIRE_EQUAL( test.intValue, 0 );
    COCA_REQUIRE_EQUAL( test.floatValue, 0 );
    COCA_REQUIRE_EQUAL( test.testValue.value, "" );

    // member functions
    coca::Accessor<int> accInt = coca::makeAccessor( test, &AccessorTestClass::getInt, &AccessorTestClass::setInt );
    accInt( 123 );
    COCA_REQUIRE_EQUAL( test.intValue, 123 );
    COCA_REQUIRE_EQUAL( accInt(), 123 );

    accInt = coca::makeAccessor( test, &AccessorTestClass::getIntConst, &AccessorTestClass::setInt );
    accInt( 1234 );
    COCA_REQUIRE_EQUAL( test.intValue, 1234 );
    COCA_REQUIRE_EQUAL( accInt(), 1234 );

    accInt = coca::makeAccessor( test, &AccessorTestClass::getInt, &AccessorTestClass::setIntConst );
    accInt( 12345 );
    COCA_REQUIRE_EQUAL( test.intValue, 12345 );
    COCA_REQUIRE_EQUAL( accInt(), 12345 );

    accInt = coca::makeAccessor( test, &AccessorTestClass::getIntConst, &AccessorTestClass::setIntConst );
    accInt( 123456 );
    COCA_REQUIRE_EQUAL( test.intValue, 123456 );
    COCA_REQUIRE_EQUAL( accInt(), 123456 );

    coca::Accessor<int,bool> accInt2 = coca::makeAccessor( test, &AccessorTestClass::getIntConst, &AccessorTestClass::setIntReturn );
    COCA_REQUIRE_EQUAL( accInt2( 2123456 ), true );
    COCA_REQUIRE_EQUAL( test.intValue, 2123456 );
    COCA_REQUIRE_EQUAL( accInt2(), 2123456 );

    accInt2 = coca::makeAccessor( test, &AccessorTestClass::getIntConst, &AccessorTestClass::setIntConstReturn );
    COCA_REQUIRE_EQUAL( accInt2( 2126 ), false );
    COCA_REQUIRE_EQUAL( test.intValue, 2126 );
    COCA_REQUIRE_EQUAL( accInt2(), 2126 );

    ATestType tt;
    COCA_REQUIRE_EQUAL( tt.value, "" );
    COCA_REQUIRE_EQUAL( test.testValue.value, "" );
    coca::Accessor<const ATestType&> accTest = makeAccessor( test, &AccessorTestClass::getTest, &AccessorTestClass::setTest );
    tt.value = "bla";
    accTest( tt );
    COCA_REQUIRE_EQUAL( test.testValue.value, "bla" );
    COCA_REQUIRE_EQUAL( accTest().value, "bla" );

    // member data
    /// @todo: check if we can make this work.
    ///accInt = coca::makeAccessor( test, &AccessorTestClass::intValue );
    //accInt( 12453 );
    //COCA_REQUIRE_EQUAL( test.intValue, 12453 );
    //COCA_REQUIRE_EQUAL( accInt(), 12453 );

    coca::Accessor<const int&> accIntRef = coca::makeAccessor( test, &AccessorTestClass::intValue );
    accIntRef( 153 );
    COCA_REQUIRE_EQUAL( test.intValue, 153 );
    COCA_REQUIRE_EQUAL( accIntRef(), 153 );

    // data
    /// @todo: check if we can make this work.
    ///accInt = coca::makeAccessor( test.intValue );
    //accInt( 124653 );
    //COCA_REQUIRE_EQUAL( test.intValue, 124653 );
    //COCA_REQUIRE_EQUAL( accInt(), 124653 );

    accIntRef = coca::makeAccessor( test.intValue );
    accIntRef( 123 );
    COCA_REQUIRE_EQUAL( test.intValue, 123 );
    COCA_REQUIRE_EQUAL( accIntRef(), 123 );

    //coca::Accessor<const ATestType&> accTest2 = coca::makeAccessor( test.testValue );
}

void testAccessors()
{
    testRefMemberFunctionAccessor();
    testPtrMemberFunctionAccessor();
    testRefMemberDataAccessor();
    testPtrMemberDataAccessor();
    testObjectFunctionAccessor();
    testObjectFunctionAccessorAdaptor();
    testObjectDataAccessor();
    testObjectDataAccessorAdaptor();
    testDataAccessor();
    testDataAccessorAdaptor();
    testAccessor();
}
