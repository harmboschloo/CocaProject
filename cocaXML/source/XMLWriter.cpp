#include "XMLWriter.h"
#include "xml_version.h"
#include <coca/ISystem.h>
#include <coca/INode.h>


XMLWriter::XMLWriter() :
        _writer( 0 )
{
}

XMLWriter::~XMLWriter()
{
}

bool XMLWriter::write( const coca::ISystem& system, std::ostream& stream )
{
    coca::XMLWriter writer( stream );
    _writer = &writer;
    writeHeader();
    write( system );
    writeFooter();
    _writer = 0;
    _ids.clear();
    return !stream.fail();
}

bool XMLWriter::write( const coca::INode& node, std::ostream& stream )
{
    coca::XMLWriter writer( stream );
    _writer = &writer;
    writeHeader();
    write( node );
    writeFooter();
    _writer = 0;
    _ids.clear();
    return !stream.fail();
}

void XMLWriter::writeHeader()
{
    _writer->writeHeader();
    _writer->writeElementStart( "coca_xml" );
    _writer->writeAttribute( "version", XML_VERSION );
}

void XMLWriter::writeFooter()
{
    _writer->writeElementEnd();
}

void XMLWriter::write( const coca::ISystem& system )
{
    _writer->writeElementStart( "system" );
    if ( !system.getDescription().empty() )
    {
        _writer->writeElementStart( "description" );
        _writer->writeText( system.getDescription() );
        _writer->writeElementEnd();
    }
    if ( system.getRoot() ) { write( *system.getRoot() ); }
    _writer->writeElementEnd();
}

void XMLWriter::write( const coca::INode& node )
{
    if ( node.isComponentNode() )
    {
        // component node
        _writer->writeElementStart( "component_node" );

        // component id
        const coca::IComponent* component = node.getComponent();
        if ( component ) { _writer->writeAttribute( "component_id", node.getComponentId() ); }
    }
    else
    {
        // composite node
        _writer->writeElementStart( "composite_node" );
    }

    // name
    _writer->writeAttribute( "name", node.getName() );

    // disabled count
    _writer->writeAttribute( "disabled_count", node.getDisabledCount() );

    if ( node.isComponentNode() )
    {
        // component node attributes
        const coca::INode::AttributeMap& attributes = node.getAttributes();
        coca::INode::AttributeMap::const_iterator it;
        for ( it = attributes.begin(); it != attributes.end(); ++it )
        {
            write( *it->second, it->first );
        }
    }
    else
    {
        // composite node child nodes
        {
            const coca::INode::NodeVector& children = node.getChildren();
            coca::INode::NodeVector::const_iterator it;
            for ( it = children.begin(); it != children.end(); ++it )
            {
                write( **it );
            }
        }

        // composite node wrapped attributes
        {
            const coca::INode::AttributeMap& attributes = node.getAttributes();
            coca::INode::AttributeMap::const_iterator it;
            for ( it = attributes.begin(); it != attributes.end(); ++it )
            {
                write( *it->second, it->first, node );
            }
        }
    }

    _writer->writeElementEnd();
}

void XMLWriter::write( const coca::IAttribute& attribute, const std::string& name )
{
    _writer->writeElementStart( "attribute" );

    // name
    _writer->writeAttribute( "name", name );

    // enabled
    _writer->writeAttribute( "enabled", attribute.isSetEnabled() );

    // auto update
    _writer->writeAttribute( "auto_update", attribute.hasAutoUpdate() );

    if ( attribute.isInputAttribute() )
    {
        // sink id
        _writer->writeAttribute( "sink_id", findId( attribute ) );

        // value
        if ( attribute.canBeSetAsString() && attribute.getSources()->empty() )
        {
            std::string value = attribute.getAsString();
            if ( !value.empty() )
            {
                _writer->writeElementStart( "value" );
                if ( value.find_last_of( '\n' ) == std::string::npos ) { _writer->setSingleLine( true ); }
                _writer->writeText( value );
                _writer->writeElementEnd();
                _writer->setSingleLine( false );
            }
        }
    }

    // sinks
    const coca::IAttribute::AttributeVector* sinks = attribute.getSinks();
    if ( sinks )
    {
        coca::IAttribute::AttributeVector::const_iterator it;
        for ( it = sinks->begin(); it != sinks->end(); ++it )
        {
            _writer->writeElementStart( "sink" );
            _writer->writeAttribute( "id", findId( **it ) );
            _writer->writeElementEnd();
        }
    }

    _writer->writeElementEnd();
}

void XMLWriter::write( const coca::IAttribute& attribute, const std::string& name,
                       const coca::INode& node )
{
    _writer->writeElementStart( "wrapped_attribute" );

    // name
    _writer->writeAttribute( "name", name );

    const coca::INode::NodeVector& children = node.getChildren();
    std::string realName;
    for ( size_t i = 0; i < children.size(); ++i )
    {
        realName = children[i]->findAttributeName( &attribute );
        if ( !realName.empty() )
        {
            // child node index
            _writer->writeAttribute( "node_index", i );
            // child node attribute name
            _writer->writeAttribute( "node_attribute_name", realName );
            break;
        }
    }

    _writer->writeElementEnd();
}

size_t XMLWriter::findId( const coca::IAttribute& attribute )
{
    for ( size_t i = 0; i < _ids.size(); ++i )
    {
        if ( _ids[i] == &attribute ) { return i; }
    }

    // new id
    _ids.push_back( &attribute );
    return ( _ids.size()-1 );
}
