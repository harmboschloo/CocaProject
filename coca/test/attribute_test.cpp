#include "attribute_test.h"
#include <coca/coca.h>

template<typename T>
class InputTestAttribute : public coca::AInputAttribute<T>
{
public:
    InputTestAttribute( bool multipleSources = false ) :
            coca::AInputAttribute<T>( multipleSources ),
            sourceAttachCount( 0 ),
            sourceDetachCount( 0 ),
            sourceUpdateCount( 0 ),
            stringUpdateCount( 0 ),
            value()
    {
    }

    virtual ~InputTestAttribute()
    {
    }

    // Input attribute callbacks
    virtual void onSourceAttach( coca::IAttribute& output )
    {
        ++sourceAttachCount;
        onSourceUpdate( output );
    }

    virtual void onSourceDetach( coca::IAttribute& COCA_HIDE( output ) )
    {
        ++sourceDetachCount;
    }

    virtual bool onSourceUpdate( coca::IAttribute& output )
    {
        ++sourceUpdateCount;
        value = this->getSourceAttribute( output ).getValue();
        return true;
    }

    virtual bool onStringUpdate( const std::string& valueString )
    {
        ++stringUpdateCount;
        return coca::fromString( valueString, value );
    }

    int sourceAttachCount;
    int sourceDetachCount;
    int sourceUpdateCount;
    int stringUpdateCount;
    T value;
};

template<typename T>
class OutputTestAttribute : public coca::AOutputAttribute<T>
{
public:
    OutputTestAttribute() :
            stringRequestCount( 0 ),
            getValueCount( 0 ),
            value()

    {
    }

    virtual ~OutputTestAttribute()
    {
    }

    // Output attribute callbacks
    virtual std::string onStringRequest() const
    {
        ++stringRequestCount;
        return coca::AOutputAttribute<T>::onStringRequest();
    }

    virtual T getValue() const
    {
        ++getValueCount;
        return value;
    }

    mutable int stringRequestCount;
    mutable int getValueCount;
    T value;

};

template<typename T>
class IOTestAttribute : public coca::AInputOutputAttribute<T>
{
public:
    IOTestAttribute( bool multipleSources = false ) :
            coca::AInputOutputAttribute<T>( multipleSources ),
            sourceAttachCount( 0 ),
            sourceDetachCount( 0 ),
            sourceUpdateCount( 0 ),
            stringUpdateCount( 0 ),
            stringRequestCount( 0 ),
            getValueCount( 0 ),
            value()

    {
    }

    virtual ~IOTestAttribute()
    {
    }

    // Input attribute callbacks
    virtual void onSourceAttach( coca::IAttribute& output )
    {
        ++sourceAttachCount;
        this->doSourceUpdate( output );
    }

    virtual void onSourceDetach( coca::IAttribute& COCA_HIDE( output ) )
    {
        ++sourceDetachCount;
    }

    virtual bool onSourceUpdate( coca::IAttribute& output )
    {
        ++sourceUpdateCount;
        value = this->getSourceAttribute( output ).getValue();
        return true;
    }

    virtual bool onStringUpdate( const std::string& valueString )
    {
        ++stringUpdateCount;
        return coca::fromString( valueString, value );
    }

    // Output attribute callbacks
    virtual std::string onStringRequest() const
    {
        ++stringRequestCount;
        return coca::AOutputAttribute<T>::onStringRequest();
    }

    virtual T getValue() const
    {
        ++getValueCount;
        return value;
    }

    int sourceAttachCount;
    int sourceDetachCount;
    int sourceUpdateCount;
    int stringUpdateCount;
    mutable int stringRequestCount;
    mutable int getValueCount;
    T value;

};

template<typename T>
class EventAttributeTestClass
{
public:
    EventAttributeTestClass():
            sourceAttachCount( 0 ),
            sourceDetachCount( 0 ),
            sourceUpdateCount( 0 ),
            stringUpdateCount( 0 ),
            value()
    {
    }

    ~EventAttributeTestClass()
    {
    }

    bool eventCallback( coca::AttributeEvent<T>& event )
    {
        switch( event.getType() )
        {
            case coca::E_SOURCE_ATTACH_EVENT:
                ++sourceAttachCount;
                // fall through
            case coca::E_SOURCE_UPDATE_EVENT:
                ++sourceUpdateCount;
                value = event.getSource()->getValue();
                break;
            case coca::E_SOURCE_DETACH_EVENT:
                ++sourceDetachCount;
                break;
            case coca::E_STRING_UPDATE_EVENT:
                ++stringUpdateCount;
                return coca::fromString( event.getValueString(), value );
        }

        return true;
    }

    int sourceAttachCount;
    int sourceDetachCount;
    int sourceUpdateCount;
    int stringUpdateCount;
    T value;
};

void basicTest()
{
    InputTestAttribute<double> iAttribute;
    iAttribute.value = 123.456;
    COCA_REQUIRE_EQUAL( iAttribute.isInputAttribute(), true );
    COCA_REQUIRE_EQUAL( iAttribute.isMultipleInputAttribute(), false );
    COCA_REQUIRE_EQUAL( iAttribute.isOutputAttribute(), false );
    COCA_REQUIRE_EQUAL( iAttribute.isSetEnabled(), true );
    COCA_REQUIRE_EQUAL( iAttribute.isEnabled(), true );
    COCA_REQUIRE_EQUAL( iAttribute.isUpdating(), false );
    COCA_REQUIRE( iAttribute.getTypeInfo() == typeid( double ) );
    COCA_REQUIRE_EQUAL( iAttribute.getAsString(), "" );
    COCA_REQUIRE( iAttribute.getSources() );
    COCA_REQUIRE_NULL( iAttribute.getSinks() );
    COCA_REQUIRE_EQUAL( iAttribute.sourceAttachCount, 0 );
    COCA_REQUIRE_EQUAL( iAttribute.sourceDetachCount, 0 );
    COCA_REQUIRE_EQUAL( iAttribute.sourceUpdateCount, 0 );
    COCA_REQUIRE_EQUAL( iAttribute.stringUpdateCount, 0 );
    COCA_REQUIRE_EQUAL( iAttribute.setAsString( "456.123" ), true );
    COCA_REQUIRE_EQUAL( iAttribute.value, 456.123 );
    COCA_REQUIRE_EQUAL( iAttribute.stringUpdateCount, 1 );


    OutputTestAttribute<double> oAttribute;
    oAttribute.value = 456.789;
    COCA_REQUIRE_EQUAL( oAttribute.isInputAttribute(), false );
    COCA_REQUIRE_EQUAL( oAttribute.isMultipleInputAttribute(), false );
    COCA_REQUIRE_EQUAL( oAttribute.isOutputAttribute(), true );
    COCA_REQUIRE_EQUAL( oAttribute.isSetEnabled(), true );
    COCA_REQUIRE_EQUAL( oAttribute.isEnabled(), true );
    COCA_REQUIRE_EQUAL( oAttribute.isUpdating(), false );
    COCA_REQUIRE( oAttribute.getTypeInfo() == typeid( double ) );
    COCA_REQUIRE_EQUAL( oAttribute.getAsString(), "456.789" );
    COCA_REQUIRE_NULL( oAttribute.getSources() );
    COCA_REQUIRE( oAttribute.getSinks() );
    COCA_REQUIRE_EQUAL( oAttribute.stringRequestCount, 1 );
    COCA_REQUIRE_EQUAL( oAttribute.getValueCount, 1 );

    IOTestAttribute<double> ioAttribute;
    ioAttribute.value = 123.789;
    COCA_REQUIRE_EQUAL( ioAttribute.isInputAttribute(), true );
    COCA_REQUIRE_EQUAL( ioAttribute.isMultipleInputAttribute(), false );
    COCA_REQUIRE_EQUAL( ioAttribute.isOutputAttribute(), true );
    COCA_REQUIRE_EQUAL( ioAttribute.isSetEnabled(), true );
    COCA_REQUIRE_EQUAL( ioAttribute.isEnabled(), true );
    COCA_REQUIRE_EQUAL( ioAttribute.isUpdating(), false );
    COCA_REQUIRE( ioAttribute.getTypeInfo() == typeid( double ) );
    COCA_REQUIRE_EQUAL( ioAttribute.getAsString(), "123.789" );
    COCA_REQUIRE( ioAttribute.getSources() );
    COCA_REQUIRE( ioAttribute.getSinks() );
    COCA_REQUIRE_EQUAL( ioAttribute.sourceAttachCount, 0 );
    COCA_REQUIRE_EQUAL( ioAttribute.sourceDetachCount, 0 );
    COCA_REQUIRE_EQUAL( ioAttribute.sourceUpdateCount, 0 );
    COCA_REQUIRE_EQUAL( ioAttribute.stringUpdateCount, 0 );
    COCA_REQUIRE_EQUAL( ioAttribute.stringRequestCount, 1 );
    COCA_REQUIRE_EQUAL( ioAttribute.getValueCount, 1 );
    COCA_REQUIRE_EQUAL( ioAttribute.setAsString( "4.123" ), true );
    COCA_REQUIRE_EQUAL( ioAttribute.value, 4.123 );
    COCA_REQUIRE_EQUAL( ioAttribute.stringUpdateCount, 1 );

    EventAttributeTestClass<double> eventTest;
    coca::EventInputAttribute<double, coca::Function<bool,coca::AttributeEvent<double>& > > eventAttribute( coca::makeFunction( eventTest, &EventAttributeTestClass<double>::eventCallback ), true );
    COCA_REQUIRE_EQUAL( eventAttribute.isInputAttribute(), true );
    COCA_REQUIRE_EQUAL( eventAttribute.isMultipleInputAttribute(), true );
    COCA_REQUIRE_EQUAL( eventAttribute.isOutputAttribute(), false );
    COCA_REQUIRE_EQUAL( eventAttribute.isSetEnabled(), true );
    COCA_REQUIRE_EQUAL( eventAttribute.isEnabled(), true );
    COCA_REQUIRE_EQUAL( eventAttribute.isUpdating(), false );
    COCA_REQUIRE( eventAttribute.getTypeInfo() == typeid( double ) );
    COCA_REQUIRE_EQUAL( eventAttribute.getAsString(), "" );
    COCA_REQUIRE( eventAttribute.getSources() );
    COCA_REQUIRE_NULL( eventAttribute.getSinks() );
    COCA_REQUIRE_EQUAL( eventTest.sourceAttachCount, 0 );
    COCA_REQUIRE_EQUAL( eventTest.sourceDetachCount, 0 );
    COCA_REQUIRE_EQUAL( eventTest.sourceUpdateCount, 0 );
    COCA_REQUIRE_EQUAL( eventTest.stringUpdateCount, 0 );
    COCA_REQUIRE_EQUAL( eventAttribute.setAsString( "456.123" ), true );
    COCA_REQUIRE_EQUAL( eventTest.value, 456.123 );
    COCA_REQUIRE_EQUAL( eventTest.stringUpdateCount, 1 );

    // connections
    COCA_REQUIRE_EQUAL( oAttribute.connectSink( iAttribute ), true );
    COCA_REQUIRE_EQUAL( oAttribute.connectSink( eventAttribute ), true );
    COCA_REQUIRE_EQUAL( oAttribute.getSinks()->size(), (size_t)2 );
    COCA_REQUIRE_EQUAL( iAttribute.sourceAttachCount, 1 );
    COCA_REQUIRE_EQUAL( iAttribute.sourceDetachCount, 0 );
    COCA_REQUIRE_EQUAL( iAttribute.sourceUpdateCount, 1 );
    COCA_REQUIRE_EQUAL( iAttribute.getSources()->size(), (size_t)1 );
    COCA_REQUIRE_EQUAL( eventTest.sourceAttachCount, 1 );
    COCA_REQUIRE_EQUAL( eventTest.sourceDetachCount, 0 );
    COCA_REQUIRE_EQUAL( eventTest.sourceUpdateCount, 1 );
    COCA_REQUIRE_EQUAL( eventAttribute.getSources()->size(), (size_t)1 );
    COCA_REQUIRE_EQUAL( oAttribute.update(), true );
    COCA_REQUIRE_EQUAL( iAttribute.sourceAttachCount, 1 );
    COCA_REQUIRE_EQUAL( iAttribute.sourceDetachCount, 0 );
    COCA_REQUIRE_EQUAL( iAttribute.sourceUpdateCount, 2 );
    COCA_REQUIRE_EQUAL( iAttribute.value, oAttribute.value );
    COCA_REQUIRE_EQUAL( eventTest.sourceAttachCount, 1 );
    COCA_REQUIRE_EQUAL( eventTest.sourceDetachCount, 0 );
    COCA_REQUIRE_EQUAL( eventTest.sourceUpdateCount, 2 );
    COCA_REQUIRE_EQUAL( eventTest.value, oAttribute.value );
    iAttribute.disconnectSources();
    COCA_REQUIRE_EQUAL( oAttribute.getSinks()->size(), (size_t)1 );
    COCA_REQUIRE_EQUAL( iAttribute.sourceAttachCount, 1 );
    COCA_REQUIRE_EQUAL( iAttribute.sourceDetachCount, 1 );
    COCA_REQUIRE_EQUAL( iAttribute.sourceUpdateCount, 2 );
    COCA_REQUIRE_EQUAL( iAttribute.getSources()->size(), (size_t)0 );

    COCA_REQUIRE_EQUAL( oAttribute.connectSink( iAttribute ), true );
    COCA_REQUIRE_EQUAL( iAttribute.sourceAttachCount, 2 );
    COCA_REQUIRE_EQUAL( iAttribute.sourceDetachCount, 1 );
    COCA_REQUIRE_EQUAL( iAttribute.sourceUpdateCount, 3 );
    COCA_REQUIRE_EQUAL( oAttribute.connectSink( ioAttribute ), true );
    COCA_REQUIRE_EQUAL( ioAttribute.connectSink( iAttribute ), true ); // should disconnect i from o
    COCA_REQUIRE_EQUAL( oAttribute.getSinks()->size(), (size_t)2 );
    COCA_REQUIRE_EQUAL( ioAttribute.sourceAttachCount, 1 );
    COCA_REQUIRE_EQUAL( ioAttribute.sourceDetachCount, 0 );
    COCA_REQUIRE_EQUAL( ioAttribute.sourceUpdateCount, 1 );
    COCA_REQUIRE_EQUAL( iAttribute.sourceAttachCount, 3 );
    COCA_REQUIRE_EQUAL( iAttribute.sourceDetachCount, 2 );
    COCA_REQUIRE_EQUAL( iAttribute.sourceUpdateCount, 4 );
    COCA_REQUIRE_EQUAL( iAttribute.getSources()->size(), (size_t)1 );
    COCA_REQUIRE_EQUAL( oAttribute.update(), true );
    COCA_REQUIRE_EQUAL( ioAttribute.sourceUpdateCount, 2 );
    COCA_REQUIRE_EQUAL( ioAttribute.value, oAttribute.value );
    COCA_REQUIRE_EQUAL( iAttribute.sourceUpdateCount, 5 );
    COCA_REQUIRE_EQUAL( iAttribute.value, ioAttribute.value );
    COCA_REQUIRE_EQUAL( iAttribute.value, oAttribute.value );
    COCA_REQUIRE_EQUAL( eventTest.sourceUpdateCount, 3 );
    COCA_REQUIRE_EQUAL( eventTest.value, oAttribute.value );

    COCA_REQUIRE_EQUAL( ioAttribute.isEnabled(), true );
    COCA_REQUIRE_EQUAL( ioAttribute.isSetEnabled(), true );
    COCA_REQUIRE_EQUAL( ioAttribute.sourceUpdateCount, 2 );
    COCA_REQUIRE_EQUAL( ioAttribute.sourceAttachCount, 1 );
    COCA_REQUIRE_EQUAL( ioAttribute.sourceDetachCount, 0 );
    COCA_REQUIRE_EQUAL( iAttribute.sourceAttachCount, 3 );
    COCA_REQUIRE_EQUAL( iAttribute.sourceDetachCount, 2 );
    COCA_REQUIRE_EQUAL( iAttribute.sourceUpdateCount, 5 );
    ioAttribute.setEnabled( false );
    COCA_REQUIRE_EQUAL( ioAttribute.isEnabled(), false );
    COCA_REQUIRE_EQUAL( ioAttribute.isSetEnabled(), false );
    COCA_REQUIRE_EQUAL( ioAttribute.sourceAttachCount, 1 );
    COCA_REQUIRE_EQUAL( ioAttribute.sourceDetachCount, 1 );
    COCA_REQUIRE_EQUAL( ioAttribute.sourceUpdateCount, 2 );
    COCA_REQUIRE_EQUAL( iAttribute.sourceAttachCount, 3 );
    COCA_REQUIRE_EQUAL( iAttribute.sourceDetachCount, 3 );
    COCA_REQUIRE_EQUAL( iAttribute.sourceUpdateCount, 5 );
    COCA_REQUIRE_EQUAL( oAttribute.value, 456.789 );
    COCA_REQUIRE_EQUAL( ioAttribute.value, 456.789 );
    COCA_REQUIRE_EQUAL( iAttribute.value, 456.789 );
    oAttribute.value = 90.12;
    COCA_REQUIRE_EQUAL( oAttribute.update(), true );
    COCA_REQUIRE_EQUAL( ioAttribute.sourceUpdateCount, 2 );
    COCA_REQUIRE_EQUAL( iAttribute.sourceUpdateCount, 5 );
    COCA_REQUIRE_EQUAL( eventTest.sourceUpdateCount, 4 );
    COCA_REQUIRE_EQUAL( ioAttribute.value, 456.789 );
    COCA_REQUIRE_EQUAL( iAttribute.value, 456.789 );
    ioAttribute.value = 90324.12;
    iAttribute.setEnabled( false );
    COCA_REQUIRE_EQUAL( iAttribute.isEnabled(), false );
    COCA_REQUIRE_EQUAL( iAttribute.sourceAttachCount, 3 );
    COCA_REQUIRE_EQUAL( iAttribute.sourceDetachCount, 3 );
    COCA_REQUIRE_EQUAL( iAttribute.sourceUpdateCount, 5 );
    iAttribute.setEnabled( true );
    COCA_REQUIRE_EQUAL( iAttribute.isEnabled(), true );
    COCA_REQUIRE_EQUAL( iAttribute.sourceAttachCount, 3 );
    COCA_REQUIRE_EQUAL( iAttribute.sourceDetachCount, 3 );
    COCA_REQUIRE_EQUAL( iAttribute.sourceUpdateCount, 5 );
    COCA_REQUIRE_EQUAL( iAttribute.value, 456.789 );
    ioAttribute.setEnabled( true );
    COCA_REQUIRE_EQUAL( ioAttribute.sourceAttachCount, 2 );
    COCA_REQUIRE_EQUAL( ioAttribute.sourceDetachCount, 1 );
    COCA_REQUIRE_EQUAL( ioAttribute.sourceUpdateCount, 3 );
    COCA_REQUIRE_EQUAL( iAttribute.sourceAttachCount, 4 );
    COCA_REQUIRE_EQUAL( iAttribute.sourceDetachCount, 3 );
    // two updates, one from ioAttribute.onSourceAttach -> ioAttribute.doSourceUpdate -> iAttribute.onSourceUpdate
    // and one from iAttribute.onSourceAttach -> iAttribute.onSourceUpdate
    COCA_REQUIRE_EQUAL( iAttribute.sourceUpdateCount, 7 );
    COCA_REQUIRE_EQUAL( ioAttribute.value, 90.12 );
    COCA_REQUIRE_EQUAL( iAttribute.value, 90.12 );

    oAttribute.disconnectSinks();
    COCA_REQUIRE_EQUAL( oAttribute.getSinks()->size(), (size_t)0 );
    COCA_REQUIRE_EQUAL( ioAttribute.sourceAttachCount, 2 );
    COCA_REQUIRE_EQUAL( ioAttribute.sourceDetachCount, 2 );
    COCA_REQUIRE_EQUAL( ioAttribute.sourceUpdateCount, 3 );
    COCA_REQUIRE_EQUAL( ioAttribute.getSources()->size(), (size_t)0 );
    COCA_REQUIRE_EQUAL( eventTest.sourceAttachCount, 1 );
    COCA_REQUIRE_EQUAL( eventTest.sourceDetachCount, 1 );
    COCA_REQUIRE_EQUAL( eventTest.sourceUpdateCount, 4 );
    COCA_REQUIRE_EQUAL( eventAttribute.getSources()->size(), (size_t)0 );

    COCA_REQUIRE_EQUAL( eventAttribute.setAsString( "1234" ), true );
    COCA_REQUIRE_EQUAL( eventTest.stringUpdateCount, 2 );
    COCA_REQUIRE_EQUAL( eventTest.value, 1234 );
    eventAttribute.setEnabled( false );
    COCA_REQUIRE_EQUAL( eventAttribute.isEnabled(), false );
    COCA_REQUIRE_EQUAL( eventTest.sourceAttachCount, 1 );
    COCA_REQUIRE_EQUAL( eventTest.sourceDetachCount, 1 );
    COCA_REQUIRE_EQUAL( eventTest.sourceUpdateCount, 4 );
    COCA_REQUIRE_EQUAL( eventAttribute.getSources()->size(), (size_t)0 );

    COCA_REQUIRE_EQUAL( eventAttribute.setAsString( "1234567" ), true );
    COCA_REQUIRE_EQUAL( eventTest.stringUpdateCount, 3 );
    COCA_REQUIRE_EQUAL( eventTest.value, 1234567 );
}

template<typename T>
class TestClass
{
public:
    TestClass() : setValueCount( 0 ), getValueCount( 0 ), value() {}
    ~TestClass() {}

    void setValue( const T& v )
    {
        value = v;
    }

    const T& getValue() const
    {
        return value;
    }

    int setValueCount;
    int getValueCount;
    T value;
};


void functionAttributeTest()
{
    // Function Attribute test

    TestClass<double> test1;
    test1.value = 432.1;
    TestClass<double> test2;
    test2.value = 2.1;
    coca::FunctionInputAttribute<double,coca::Function<void,const double&> > iAttribute( coca::makeFunction( test1, &TestClass<double>::setValue ) );
    coca::FunctionOutputAttribute<double,coca::Function<const double&> > oAttribute( coca::makeFunction( test2, &TestClass<double>::getValue ) );

    COCA_REQUIRE_EQUAL( iAttribute.connectSource( oAttribute ), true );
    COCA_REQUIRE_EQUAL( test1.value, 2.1 );
    COCA_REQUIRE_EQUAL( test1.value, test2.value );
}

void pointerFunctionAttributeTest()
{
    // Function Attribute pointer test

    double value = 123345.6789;
    TestClass<double*> test1;
    test1.value = 0;
    TestClass<double*> test2;
    test2.value = &value;
    coca::FunctionInputAttribute<double*,coca::Function<void,double* const&> > iAttribute( coca::makeFunction( test1, &TestClass<double*>::setValue ) );
    coca::FunctionOutputAttribute<double*,coca::Function<double* const&> > oAttribute( coca::makeFunction( test2, &TestClass<double*>::getValue ) );

    COCA_REQUIRE_EQUAL( iAttribute.connectSource( oAttribute ), true );
    COCA_REQUIRE_EQUAL( test1.value, &value );
    COCA_REQUIRE_EQUAL( test1.value, test2.value );
    iAttribute.disconnectSources(); // should reset input pointer to 0!
    COCA_REQUIRE_NULL( test1.value );

    TestClass<double*> test3;
    test3.value = 0;

    coca::AccessorInputOutputAttribute<double*,coca::Accessor<double* const&> > ioAttribute( coca::makeAccessor( test3, &TestClass<double*>::getValue, &TestClass<double*>::setValue ) );
    COCA_REQUIRE_EQUAL( oAttribute.connectSink( ioAttribute ), true );
    COCA_REQUIRE_EQUAL( ioAttribute.connectSink( iAttribute ), true );
    COCA_REQUIRE_EQUAL( oAttribute.getSinks()->size(), (size_t)1 );
    COCA_REQUIRE_EQUAL( ioAttribute.getSources()->size(), (size_t)1 );
    COCA_REQUIRE_EQUAL( ioAttribute.getSinks()->size(), (size_t)1 );
    COCA_REQUIRE_EQUAL( iAttribute.getSources()->size(), (size_t)1 );
    COCA_REQUIRE_EQUAL( test1.value, &value );
    COCA_REQUIRE_EQUAL( test2.value, &value );
    COCA_REQUIRE_EQUAL( test3.value, &value );

    ioAttribute.setEnabled( false );
    COCA_REQUIRE_EQUAL( ioAttribute.isEnabled(), false );
    COCA_REQUIRE_NULL( test3.value );
    COCA_REQUIRE_NULL( test1.value );
    ioAttribute.setEnabled( true );
    COCA_REQUIRE_EQUAL( test3.value, &value );
    COCA_REQUIRE_EQUAL( test1.value, &value );
    ioAttribute.disconnectSources();
    COCA_REQUIRE_NULL( test3.value );
    COCA_REQUIRE_NULL( test1.value );

}

void createAttributeTest()
{
    TestClass<double> test;
    coca::ATypeAttribute<double>* attribute = 0;

    // FunctionInputAttribute
    attribute = coca::createInputAttribute<double>( coca::makeFunction( test, &TestClass<double>::setValue ) );
    COCA_REQUIRE_EQUAL( attribute->isInputAttribute(), true );
    COCA_REQUIRE_EQUAL( attribute->isMultipleInputAttribute(), false );
    COCA_REQUIRE_EQUAL( attribute->isOutputAttribute(), false );
    delete attribute;

    // FunctionOutputAttribute
    attribute = coca::createOutputAttribute<double>( coca::makeFunction( test, &TestClass<double>::getValue ) );
    COCA_REQUIRE_EQUAL( attribute->isInputAttribute(), false );
    COCA_REQUIRE_EQUAL( attribute->isMultipleInputAttribute(), false );
    COCA_REQUIRE_EQUAL( attribute->isOutputAttribute(), true );
    delete attribute;

    // AccessorInputOutputAttribute
    attribute = coca::createInputOutputAttribute<double>( coca::makeAccessor( test, &TestClass<double>::getValue, &TestClass<double>::setValue ) );
    COCA_REQUIRE_EQUAL( attribute->isInputAttribute(), true );
    COCA_REQUIRE_EQUAL( attribute->isMultipleInputAttribute(), false );
    COCA_REQUIRE_EQUAL( attribute->isOutputAttribute(), true );
    delete attribute;

    attribute = coca::createInputOutputAttribute<double>( coca::makeAccessor( test, &TestClass<double>::value ) );
    COCA_REQUIRE_EQUAL( attribute->isInputAttribute(), true );
    COCA_REQUIRE_EQUAL( attribute->isMultipleInputAttribute(), false );
    COCA_REQUIRE_EQUAL( attribute->isOutputAttribute(), true );
    delete attribute;

    attribute = coca::createInputOutputAttribute<double>( coca::makeAccessor( test.value ) );
    COCA_REQUIRE_EQUAL( attribute->isInputAttribute(), true );
    COCA_REQUIRE_EQUAL( attribute->isMultipleInputAttribute(), false );
    COCA_REQUIRE_EQUAL( attribute->isOutputAttribute(), true );
    delete attribute;

    // EventInputAttribute
    EventAttributeTestClass<double> eventTest;

    attribute = coca::createInputAttribute<double>( coca::makeFunction( eventTest, &EventAttributeTestClass<double>::eventCallback ), true );
    COCA_REQUIRE_EQUAL( attribute->isInputAttribute(), true );
    COCA_REQUIRE_EQUAL( attribute->isMultipleInputAttribute(), true );
    COCA_REQUIRE_EQUAL( attribute->isOutputAttribute(), false );
    delete attribute;
}

void testAttributes()
{
    basicTest();
    functionAttributeTest();
    pointerFunctionAttributeTest();
    createAttributeTest();
}
