#include "utils_string_test.h"
#include <coca/utils.h>

class Timer
{
public:
    Timer() : _clock( clock() ) {}
    ~Timer() {}

    void reset() { _clock = clock(); }

    clock_t getTicks() const { return _clock; }
    float getSeconds() const { return _clock/float( CLOCKS_PER_SEC ); }

    clock_t getTicksPassed() const { return ( clock()-_clock ); }
    float getSecondsPassed() const { return getTicksPassed()/float( CLOCKS_PER_SEC ); }

private:
    clock_t _clock;
};

void testRemoveCharacters( const std::string& str, const std::string& characters,
                           const std::string& control, size_t loops )
{
    Timer timer;
    for ( size_t i = 0; i < loops; ++i )
    {
        std::string result( str );
        coca::removeCharacters( result, characters );
        COCA_REQUIRE_EQUAL_SILENT( result, control );
    }
    float seconds = timer.getSecondsPassed()/loops;
    COCA_INFO( "removeCharacters in " << seconds << " seconds" ); //; removed characters '" << characters
                 //<< "' from '" << str << "'" );
}

void testReplaceCharacter( const std::string& str, char character, const std::string& replacement,
                           const std::string& control, size_t loops )
{
    Timer timer;
    for ( size_t i = 0; i < loops; ++i )
    {
        std::string result( str );
        coca::replaceCharacter( result, character, replacement );
        COCA_REQUIRE_EQUAL_SILENT( result, control );
    }
    float seconds = timer.getSecondsPassed()/loops;
    COCA_INFO( "replaceCharacter in " << seconds << " seconds" ); //; replaced character '" << character
                 //<< "' with '" << replacement << "' from '" << str<< "'" );
}

template<typename T>
void testToFromString( const T& value )
{
    std::string valueString = coca::toString( value );
    T value2;
    bool ok = coca::fromString( valueString, value2 );
    COCA_REQUIRE( ok );
    COCA_REQUIRE_EQUAL( value, value2 );
}

void testStringOperations()
{
    testRemoveCharacters( "test string bla bla bla bla bla bla bla blat", "tse",
                          " ring bla bla bla bla bla bla bla bla", 100000 );

    testReplaceCharacter( "test string bla bla bla bla bla bla bla blat", 't', "bla",
                          "blaesbla sblaring bla bla bla bla bla bla bla blabla", 100000 );

    int intTest = 123;
    testToFromString( intTest );

    float floatTest = 123.456f;
    testToFromString( floatTest );

    std::string stringTest( "Hello World!" );
    testToFromString( stringTest );
}

