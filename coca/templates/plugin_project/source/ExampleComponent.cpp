#include "ExampleComponent.h"

ExampleComponent::ExampleComponent() :
        _setting( false ),
        _output( 0 )
{
}

ExampleComponent::~ExampleComponent()
{
}

bool ExampleComponent::init( coca::INode& node )
{
    // Add component attributes to the node here.
    node.addAttribute( "setting", coca::createInputOutputAttribute<bool>( coca::makeAccessor( _setting ) ) );

    coca::IAttribute* inputAttribute = coca::createInputAttribute<float>(
                                           coca::makeFunction( *this, &ExampleComponent::setInput ) );
    node.addAttribute( "input", inputAttribute );

    coca::IAttribute* outputAttribute = coca::createOutputAttribute<double>(
                                            coca::makeFunction( *this, &ExampleComponent::getOutput ) );
    node.addAttribute( "output", outputAttribute );
    coca::makeFunction<void>( *outputAttribute, &coca::IAttribute::update );
    inputAttribute->appendUpdateCallback( coca::makeFunction<void>( *outputAttribute,  &coca::IAttribute::update ) );

    // all ok
    return true;
}

void ExampleComponent::onEnabled()
{
    // Do any (re)initializations here.
}

void ExampleComponent::onDisabled()
{
    // Do any clean up here.
}

void ExampleComponent::setInput( float value )
{
    _output = 2*value;
}

double ExampleComponent::getOutput() const
{
    return _output;
}
