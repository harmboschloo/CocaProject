#include "XMLReader.h"
#include "xml_version.h"
#include <coca/ISystem.h>
#include <coca/INode.h>
#include <coca/IFactory.h>
#include <limits>
#include <memory>

XMLReader::XMLReader() :
        _reader( 0 )
{
}

XMLReader::~XMLReader()
{
}

coca::ISystem* XMLReader::readSystem( std::istream& stream )
{
    reset();
    coca::IrrXMLReader reader( stream );
    _reader = &reader;
    std::auto_ptr<coca::ISystem> system;
    if ( readHeader() )
    {
        system.reset( readSystem() );
        if ( system.get() )
        {
            readFooter();
            connectSinks();
            if ( system->getRoot() ) { system->getRoot()->enable( true ); }
        }
    }
    _reader = 0;
    reset();
    return system.release();
}

coca::INode* XMLReader::readNode( std::istream& stream )
{
    reset();
    coca::IrrXMLReader reader( stream );
    _reader = &reader;
    std::auto_ptr<coca::INode> node;
    if ( readHeader() )
    {
        while ( _reader->read() )
        {
            if ( _reader->atElementStart( "component_node" ) )
            {
                node.reset( readComponentNode() );
                break;
            }
            else if ( _reader->atElementStart( "composite_node" ) )
            {
                node.reset( readCompositeNode() );
                break;
            }
        }

        if ( node.get() )
        {
            readFooter();
            connectSinks();
            node->enable( true );
        }
    }
    _reader = 0;
    reset();
    return node.release();
}

bool XMLReader::readHeader()
{
    if ( !_reader->readUntillElementStart( "coca_xml" ) )
    {
        COCA_ERROR( "Could not find element 'coca_xml'!" );
        return false;
    }

    int version = _reader->getAttributeAsInt( "version" );
    if ( version != XML_VERSION )
    {
        COCA_ERROR( "Invalid coca_xml version!" );
        return false;
    }

    return true;
}

bool XMLReader::readFooter()
{
    if ( !_reader->readUntillElementEnd( "coca_xml" ) )
    {
        COCA_ERROR( "Did not find end of 'coca_xml' tag. Data might be corrupt." );
        return false;
    }
    return true;
}

coca::ISystem* XMLReader::readSystem()
{
    if ( !_reader->readUntillElementStart( "system" ) )
    {
        COCA_ERROR( "Could not find element 'system'!" );
        return 0;
    }

    // use auto_ptr to ensure deletion when an exception is thrown
    std::auto_ptr<coca::ISystem> system( coca::getFactory().createSystem() );

    while ( _reader->read() )
    {
        if ( _reader->atElementStart( "description" ) )
        {
            system->setDescription( readSystemDescription() );
        }
        else if ( _reader->atElementStart( "component_node" ) )
        {
            if ( system->getRoot() )
            {
                COCA_ERROR( "Multiple system roots found!" );
                return 0;
            }
            system->setRoot( readComponentNode() );
            if ( !system->getRoot() ) { return 0; }
        }
        else if ( _reader->atElementStart( "composite_node" ) )
        {
            if ( system->getRoot() )
            {
                COCA_ERROR( "Multiple system roots found!" );
                return 0;
            }
            system->setRoot( readCompositeNode() );
            if ( !system->getRoot() ) { return 0; }
        }
        else if ( _reader->atElementEnd( "system" ) )
        {
            return system.release();
        }
    }

    COCA_ERROR( "Did not find end of 'system' tag. Data might be corrupt." );
    return 0;
}

std::string XMLReader::readSystemDescription()
{
    std::string description;

    if ( !_reader->readUntillElementStart( "description" ) )
    {
        COCA_ERROR( "Could not find element 'description'!" );
        return description;
    }

    while ( _reader->read() )
    {
        if ( _reader->atText() )
        {
            description = _reader->getText();
        }
        else if ( _reader->atElementEnd( "description" ) )
        {
            return description;
        }
    }

    COCA_ERROR( "Did not find end of 'description' tag. Data might be corrupt." );
    return description;
}

coca::INode* XMLReader::readComponentNode()
{
    if ( !_reader->readUntillElementStart( "component_node" ) )
    {
        COCA_ERROR( "Could not find element 'component_node'!" );
        return 0;
    }

    // use auto_ptr to ensure deletion
    std::auto_ptr<coca::INode> node( coca::getFactory().createComponentNode() );

    readNodeAttributes( *node );

    // component
    std::string componentId = _reader->getAttribute( "component_id" );
    if ( !componentId.empty() )
    {
        node->resetComponent( componentId );
        if ( !node->getComponent() ) { return 0; }
    }

    while ( _reader->read() )
    {
        if ( _reader->atElementStart( "attribute" ) )
        {
            readAttribute( *node );
        }
        else if ( _reader->atElementEnd( "component_node" ) )
        {
            return node.release();
        }
    }

    COCA_ERROR( "Did not find end of 'component_node' tag. Data might be corrupt." );
    return 0;
}

coca::INode* XMLReader::readCompositeNode()
{
    if ( !_reader->readUntillElementStart( "composite_node" ) )
    {
        COCA_ERROR( "Could not find element 'composite_node'!" );
        return 0;
    }

    // use auto_ptr to ensure deletion
    std::auto_ptr<coca::INode> node( coca::getFactory().createCompositeNode() );

    readNodeAttributes( *node );

    while ( _reader->read() )
    {

        if ( _reader->atElementStart( "composite_node" ) )
        {
            if ( !node->append( readCompositeNode() ) )
            {
                COCA_ERROR( "Error while reading child composite node" );
                return 0;
            }
        }
        else if ( _reader->atElementStart( "component_node" ) )
        {
            if ( !node->append( readComponentNode() ) )
            {
                COCA_ERROR( "Error while reading child component node" );
                return 0;
            }
        }
        else if ( _reader->atElementStart( "wrapped_attribute" ) )
        {
            readWrapperAttribute( *node );
        }
        else if ( _reader->atElementEnd( "composite_node" ) )
        {
            return node.release();
        }
    }

    COCA_ERROR( "Did not find end of 'composite_node' tag. Data might be corrupt." );
    return 0;
}

void XMLReader::readNodeAttributes( coca::INode& node )
{
    // name
    node.setName( _reader->getAttribute( "name" ) );

    // disabled count; always initialize node disabled; it will be enabled in readSystem or readNode above
    int disabledCount = _reader->getAttributeAsInt( "disabled_count" ) + 1;
    node.setDisabledCount( disabledCount );
}

bool XMLReader::readAttribute( coca::INode& node )
{
    COCA_ASSERT( node.isComponentNode() );

    if ( !_reader->readUntillElementStart( "attribute" ) )
    {
        COCA_ERROR( "Could not find element 'attribute'!" );
        return false;
    }

    // name
    std::string name = _reader->getAttribute( "name" );
    if ( name.empty() )
    {
        COCA_WARNING( "No name specified for attribute of node " << node.getName() );
        return false;
    }

    // attribute
    coca::IAttribute* attribute = node.findAttribute( name );
    if ( !attribute )
    {
        COCA_WARNING( "Could not find attribute with name " << name << " of node " << node.getName() );
        return false;
    }

    // enabled, when not defined enabled = true
    attribute->setEnabled( _reader->getAttributeAsInt( "enabled", 1 ) > 0 );

    // auto update, when not defined auto update = true
    attribute->setAutoUpdate( _reader->getAttributeAsInt( "auto_update", 1 ) > 0 );

    if ( attribute->isInputAttribute() )
    {
        // sink id
        std::string sinkId = _reader->getAttribute( "sink_id" );
        if ( !sinkId.empty() ) { storeSink( sinkId, *attribute ); }
    }

    while ( _reader->read() )
    {
        if ( _reader->atElementStart( "value" ) )
        {
            if ( attribute->canBeSetAsString() )
            {
                if ( !readInputValue( *attribute ) )
                {
                    COCA_ERROR( "Could read input value for attribute " << name << " of node " << node.getName() );
                }
            }
        }
        else if ( _reader->atElementStart( "sink" ) )
        {
            if ( attribute->isOutputAttribute() )
            {
                readSink( *attribute );
            }
        }
        else if ( _reader->atElementEnd( "attribute" ) )
        {
            return true;
        }
    }

    COCA_ERROR( "Did not find end of 'attribute' tag. Data might be corrupt." );
    return false;
}

bool XMLReader::readInputValue( coca::IAttribute& attribute )
{
    if ( !_reader->readUntillElementStart( "value" ) )
    {
        COCA_ERROR( "Could not find element 'value'!" );
        return false;
    }

    while ( _reader->read() )
    {
        if ( _reader->atText() )
        {
            std::string valueString = _reader->getText();
            if ( !attribute.setAsString( valueString ) )
            {
                COCA_ERROR( "Could not set attribute value as string '" << valueString << "'." );
                return false;
            }
        }
        else if ( _reader->atElementEnd( "value" ) )
        {
            return true;
        }
    }

    COCA_ERROR( "Did not find end of 'value' tag. Data might be corrupt." );
    return false;
}

bool XMLReader::readSink( coca::IAttribute& attribute )
{
    if ( !_reader->readUntillElementStart( "sink" ) )
    {
        COCA_ERROR( "Could not find element 'sink'!" );
        return false;
    }

    // id
    std::string id = _reader->getAttribute( "id" );
    if ( !id.empty() ) { storeConnection( attribute, id ); }

    while ( _reader->read() )
    {
        if ( _reader->atElementEnd( "sink" ) )
        {
            return true;
        }
    }

    COCA_ERROR( "Did not find end of 'sink' tag. Data might be corrupt." );
    return false;
}

bool XMLReader::readWrapperAttribute( coca::INode& node )
{
    COCA_ASSERT( node.isCompositeNode() );

    if ( !_reader->readUntillElementStart( "wrapped_attribute" ) )
    {
        COCA_ERROR( "Could not find element 'wrapped_attribute'!" );
        return false;
    }

    // name
    std::string name = _reader->getAttribute( "name" );
    if ( name.empty() )
    {
        COCA_WARNING( "No 'name' specified for wrapper attribute of node " << node.getName() );
        return false;
    }

    // node_index
    if ( !_reader->hasAttribute( "node_index" ) )
    {
        COCA_WARNING( "No 'node_index' specified for wrapper attribute of node " << node.getName() );
        return false;
    }

    size_t index = _reader->getAttributeAsInt( "node_index" );
    if ( index >= node.getChildren().size() )
    {
        COCA_WARNING( "Invalid 'node_index' specified for wrapper attribute of node " << node.getName() );
        return false;
    }

    // node_attribute_name
    if ( !_reader->hasAttribute( "node_attribute_name" ) )
    {
        COCA_WARNING( "No 'node_attribute_name' specified for wrapper attribute of node " << node.getName() );
        return false;
    }

    std::string realName = _reader->getAttribute( "node_attribute_name" );

    coca::IAttribute* attribute = node.getChildren()[index]->findAttribute( realName );
    if ( !attribute )
    {
        COCA_WARNING( "Could not find attribute with name " << realName << " of child "
                      << index << " of node " << node.getName() );
        return false;
    }

    node.addAttribute( name, attribute );

    // read until end
    while ( _reader->read() )
    {
        if ( _reader->atElementEnd( "wrapped_attribute" ) )
        {
            return true;
        }
    }

    COCA_ERROR( "Did not find end of 'wrapped_attribute' tag. Data might be corrupt." );
    return false;
}

bool XMLReader::storeSink( std::string id, coca::IAttribute& sink )
{
    if ( id.empty() ) { return false; }

    AttributeMap::iterator it = _sinks.find( id );

    if ( it != _sinks.end() )
    {
        COCA_WARNING( "Found sink with same id (" << id << ") as another sink!" );
        return false;
    }

    _sinks[id] = &sink;
    return true;
}

bool XMLReader::storeConnection( coca::IAttribute& source, const std::string& sinkId )
{
    if ( sinkId.empty() ) { return false; }
    _connections.push_back( std::make_pair( &source, sinkId ) );
    return true;
}

void XMLReader::connectSinks()
{
    ConnectionVector::const_iterator it;
    for ( it = _connections.begin(); it != _connections.end(); ++it )
    {
        connectSink( *( it->first ), it->second );
    }
}

void XMLReader::connectSink( coca::IAttribute& source, const std::string& sinkId )
{
    AttributeMap::const_iterator it = _sinks.find( sinkId );
    if ( it == _sinks.end() )
    {
        COCA_WARNING( "Could not connect input, no input found with sink_id " << sinkId );
        return;
    }

    if ( !source.connectSink( *it->second ) )
    {
        COCA_ERROR( "Could not connect input with sink_id " << sinkId );
        return;
    }
}

void XMLReader::reset()
{
    _connections.clear();
    _sinks.clear();
}
