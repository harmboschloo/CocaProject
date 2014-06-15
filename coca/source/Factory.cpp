// Copyright (C) 2008-2009 Harm Boschloo

#include "Factory.h"
#include "System.h"
#include "ComponentNode.h"
#include "CompositeNode.h"
#include <coca/log.h>

namespace coca
{
    Factory::Factory()
    {
    }

    Factory::~Factory()
    {
    }

    // System //

    ISystem* Factory::createSystem( const std::string& description, INode* root ) const
    {
        return new System( description, root );
    }

    // Nodes //

    INode* Factory::createComponentNode( const std::string& componentId,
                                         const std::string& name, INode* parent ) const
    {
        return new ComponentNode( componentId, name, parent );
    }

    INode* Factory::createCompositeNode( const std::string& name, INode* parent ) const
    {
        return new CompositeNode( name, parent );
    }

    // Components //

    bool Factory::registerComponent( const std::string& id, ComponentRegistry::Creator creator )
    {
        if ( !_componentRegistry.add( id, creator ) )
        {
            COCA_ERROR( "A component registration for id '" << id << "' already exists!" );
            return false;
        }
        return true;
    }

    void Factory::clearComponentRegistrations()
    {
        _componentRegistry.clearRegistrations();
    }

    IComponent* Factory::createComponent( const std::string& id ) const
    {
        IComponent* component = _componentRegistry.create( id );
        if ( !component )
        {
            COCA_ERROR( "No component registration found for component id '" << id << "'!" );
        }
        return component;
    }

    const IFactory::ComponentRegistry& Factory::getComponentRegistry() const
    {
        return _componentRegistry;
    }

    // Readers //

    bool Factory::registerReader( const std::string& extension, ReaderRegistry::Creator creator )
    {
        if ( !_readerRegistry.add( extension, creator ) )
        {
            COCA_ERROR( "A registration for reader extension '" << extension << "' already exists!" );
            return false;
        }
        return true;
    }

    void Factory::clearReaderRegistrations()
    {
        _readerRegistry.clearRegistrations();
    }

    IReader* Factory::createReader( const std::string& extension ) const
    {
        IReader* reader = _readerRegistry.create( extension );
        if ( !reader )
        {
            COCA_ERROR( "No reader registered for extension '" << extension << "'!" );
        }
        return reader;
    }

    const IFactory::ReaderRegistry& Factory::getReaderRegistry() const
    {
        return _readerRegistry;
    }

    // Writers //

    bool Factory::registerWriter( const std::string& extension, WriterRegistry::Creator creator )
    {
        if ( !_writerRegistry.add( extension, creator ) )
        {
            COCA_ERROR( "A registration for writer extension '" << extension << "' already exists!" );
            return false;
        }
        return true;
    }

    void Factory::clearWriterRegistrations()
    {
        _writerRegistry.clearRegistrations();
    }

    IWriter* Factory::createWriter( const std::string& extension ) const
    {
        IWriter* writer = _writerRegistry.create( extension );
        if ( !writer )
        {
            COCA_ERROR( "No writer registered for extension '" << extension << "'!" );
        }
        return writer;
    }

    const IFactory::WriterRegistry& Factory::getWriterRegistry() const
    {
        return _writerRegistry;
    }
}
