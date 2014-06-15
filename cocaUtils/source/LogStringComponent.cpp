#include "LogStringComponent.h"
#include <coca/INode.h>
#include <coca/attributes.h>

LogStringComponent::LogStringComponent()
{
}

LogStringComponent::~LogStringComponent()
{
}

bool LogStringComponent::init( coca::INode& node )
{
    node.addAttribute( "prefix", coca::createInputOutputAttribute<std::string>(
                           coca::makeAccessor( _prefix ) ) );

    node.addAttribute( "suffix", coca::createInputOutputAttribute<std::string>(
                           coca::makeAccessor( _suffix ) ) );

    node.addAttribute( "string", coca::createInputAttribute<std::string>(
                           coca::makeFunction( *this, &LogStringComponent::log ) ) );

    return true;
}

void LogStringComponent::onEnabled()
{
}

void LogStringComponent::onDisabled()
{
}

void LogStringComponent::log( std::string data )
{
    COCA_INFO( _prefix << data << _suffix );
}
