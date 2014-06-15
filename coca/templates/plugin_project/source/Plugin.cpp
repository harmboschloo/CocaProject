#include "Plugin.h"
#include "ExampleComponent.h"

// This allows the framework to load the plugin dynamically, so it's important!
COCA_EXPORT_PLUGIN( Plugin );

Plugin::Plugin()
{
    // provide plugin information here
    // see coca::IPlugin and coca::PluginInfo
    _info.getVersion().set( 0, 0, 1, "alpha" );
    _info.setName( "cocaExamplePlugin" );
    _info.setAuthor( "H.W. Boschloo" );
    _info.setEmail( "harm@boschloo.net" );
    _info.setWebsite( "www.boschloo.net" );
    _info.setDescription( "An example plugin for the coca library." );
}

Plugin::~Plugin()
{
}

bool Plugin::init()
{
    // Register all your component, readers, writers, etc. here.
    // example:
    coca::IFactory& factory = coca::getFactory();
    factory.registerComponent<ExampleComponent>( "exampleComponent" );

    // initialization was successful
    return true;
}

