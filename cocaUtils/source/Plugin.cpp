#include "Plugin.h"
#include "LogStringComponent.h"
#include <coca/IFactory.h>
#include <coca/additional/components.h>
#include <string>

// This allows the framework to load the plugin dynamically, so it's important!
COCA_EXPORT_PLUGIN( Plugin );

Plugin::Plugin()
{
    // provide plugin information here
    // see mf::IPlugin and mf::PluginInfo
    _info.getVersion().set( 0, 0, 1, "alpha" );
    _info.setName( "cocaUtils" );
    _info.setAuthor( "H.W. Boschloo" );
    _info.setEmail( "harm@boschloo.net" );
    _info.setWebsite( "www.boschloo.net" );
    _info.setDescription( "An utilities plugin for the coca library." );
}

Plugin::~Plugin()
{
}

bool Plugin::init()
{
    // Register all your component, readers, writers, etc. here.
    // example:
    coca::IFactory& factory = coca::getFactory();
    factory.registerComponent<coca::TimerComponent<coca::float32> >( "uTimer" );
    factory.registerComponent<coca::FrameRateRecorderComponent<coca::float32> >( "uFrameRateRecorder" );
    factory.registerComponent<coca::ConstantComponent<coca::int8> >( "uConstantInt8" );
    factory.registerComponent<coca::ConstantComponent<coca::int16> >( "uConstantInt16" );
    factory.registerComponent<coca::ConstantComponent<coca::int32> >( "uConstantInt32" );
    factory.registerComponent<coca::ConstantComponent<coca::int64> >( "uConstantInt64" );
    factory.registerComponent<coca::ConstantComponent<coca::uint8> >( "uConstantUnsignedInt8" );
    factory.registerComponent<coca::ConstantComponent<coca::uint16> >( "uConstantUnsignedInt16" );
    factory.registerComponent<coca::ConstantComponent<coca::uint32> >( "uConstantUnsignedInt32" );
    factory.registerComponent<coca::ConstantComponent<coca::uint64> >( "uConstantUnsignedInt64" );
    factory.registerComponent<coca::ConstantComponent<coca::float32> >( "uConstantFloat32" );
    factory.registerComponent<coca::ConstantComponent<coca::float64> >( "uConstantFloat64" );
    factory.registerComponent<coca::ConstantComponent<std::string> >( "uConstantString" );
    factory.registerComponent<coca::ToStringComponent<coca::int8> >( "uInt8ToString" );
    factory.registerComponent<coca::ToStringComponent<coca::int16> >( "uInt16ToString" );
    factory.registerComponent<coca::ToStringComponent<coca::int32> >( "uInt32ToString" );
    factory.registerComponent<coca::ToStringComponent<coca::int64> >( "uInt64ToString" );
    factory.registerComponent<coca::ToStringComponent<coca::uint8> >( "uUnsignedInt8ToString" );
    factory.registerComponent<coca::ToStringComponent<coca::uint16> >( "uUnsignedInt16ToString" );
    factory.registerComponent<coca::ToStringComponent<coca::uint32> >( "uUnsignedInt32ToString" );
    factory.registerComponent<coca::ToStringComponent<coca::uint64> >( "uUnsignedInt64ToString" );
    factory.registerComponent<coca::ToStringComponent<coca::float32> >( "uFloat32ToString" );
    factory.registerComponent<coca::ToStringComponent<coca::float64> >( "uFloat64ToString" );
    factory.registerComponent<LogStringComponent>( "uLogString" );

    // initialization was successful
    return true;
}

