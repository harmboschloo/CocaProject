// Copyright (C) 2007-2008 Harm Boschloo

#include "coca_commands.h"
#include <coca/IFactory.h>
#include <coca/IClock.h>
#include <coca/IReader.h>
#include <coca/IWriter.h>
#include <coca/attributes/IAttribute.h>
#include <fstream>

// CompositeCommand //

bool CompositeCommand::doExecute()
{
    bool ok = true;

    std::vector<CommandPtr>::iterator it;
    for ( it = _commands.begin(); it != _commands.end(); ++it )
    {
        ok &= ( *it )->execute();
    }

    return ok;
}

void CompositeCommand::append( const CommandPtr& command )
{
    _commands.push_back( command );
}

// ClockTickCommand //

bool ClockTickCommand::doExecute()
{
    coca::getClock().tick();
    return true;
}

// ResetClockCommand //

bool ResetClockCommand::doExecute()
{
    COCA_DEBUG_INFO( "ResetClockCommand::doExecute" );
    coca::getClock().reset();
    return true;
}

// CreateSystemCommand //

CreateSystemCommand::CreateSystemCommand( coca::ISystem** system ) :
        _system( system )
{
    COCA_ASSERT( _system );
    COCA_ASSERT_EQUAL( *_system, ( void* )0 );
}

CreateSystemCommand::~CreateSystemCommand()
{
}

bool CreateSystemCommand::doExecute()
{
    COCA_DEBUG_INFO( "CreateSystemCommand::doExecute " << _system );

    ( *_system ) = coca::getFactory().createSystem();

    return true;
}

// DeleteSystemCommand //

DeleteSystemCommand::DeleteSystemCommand( coca::ISystem* system ) :
        _system( system )
{
}

DeleteSystemCommand::~DeleteSystemCommand()
{
}

bool DeleteSystemCommand::doExecute()
{
    COCA_DEBUG_INFO( "DeleteSystemCommand::doExecute " << _system );

    delete _system;

    COCA_DEBUG_INFO( "end DeleteSystemCommand::doExecute " << _system );

    _system = 0;

    return true;
}

// SaveSystemCommand //

SaveSystemCommand::SaveSystemCommand( const coca::ISystem& system, const std::string& filename ) :
        _system( &system ), _filename( filename )
{
}

SaveSystemCommand::~SaveSystemCommand()
{
}

bool SaveSystemCommand::doExecute()
{
    COCA_DEBUG_INFO( "SaveSystemCommand::doExecute" );

    // find writer
    coca::IWriter* writer = coca::getFactory().createWriter( coca::extractExtension( _filename ) );
    if ( !writer )
    {
        COCA_ERROR( "No writer found for file " << _filename );
        return false;
    }

    // open stream
    std::ofstream fileStream( _filename.c_str() );
    if ( !fileStream.is_open() )
    {
        COCA_ERROR( "Could not open file " << _filename );
        return false;
    }

    if ( !writer->write( *_system, fileStream ) )
    {
        COCA_ERROR( "Could not write system to file " << _filename );
        return false;
    }

    return true;
}

// LoadSystemCommand //

LoadSystemCommand::LoadSystemCommand( coca::ISystem** system, const std::string& filename ) :
        _system( system ),
        _filename( filename )
{
    COCA_ASSERT( _system );
    COCA_ASSERT_EQUAL( *_system, ( void* )0 );
}

LoadSystemCommand::~LoadSystemCommand()
{
}

bool LoadSystemCommand::doExecute()
{
    COCA_DEBUG_INFO( "LoadSystemCommand::doExecute" );

    // find reader
    coca::IReader* reader = coca::getFactory().createReader( coca::extractExtension( _filename ) );
    if ( !reader )
    {
        COCA_ERROR( "No reader found for file " << _filename );
        return false;
    }

    // open stream
    std::ifstream fileStream( _filename.c_str() );
    if ( !fileStream.is_open() )
    {
        COCA_ERROR( "Could not open file " << _filename );
        return false;
    }

    ( *_system ) = reader->readSystem( fileStream );

    if ( !( *_system ) )
    {
        COCA_ERROR( "Could not read system from file " << _filename );
        return false;
    }

    return true;
}

// ReadNodeCommand //

ReadNodeCommand::ReadNodeCommand( const std::string& text, coca::IReader& reader, coca::INode** node ) :
        _text( text ),
        _reader( &reader ),
        _node( node )
{
    COCA_ASSERT( _node );
    COCA_ASSERT_EQUAL( *_node, ( void* )0 );
}

ReadNodeCommand::~ReadNodeCommand()
{
}

bool ReadNodeCommand::doExecute()
{
    COCA_DEBUG_INFO( "ReadNodeCommand::doExecute" );

    std::istringstream ss( _text );
    ( *_node ) = _reader->readNode( ss );

    return (( *_node ) != 0 );
}

// SetSystemDescriptionCommand //

SetSystemDescriptionCommand::SetSystemDescriptionCommand( coca::ISystem& system, const std::string& description ) :
        _system( &system ),
        _description( description )
{
}

SetSystemDescriptionCommand::~SetSystemDescriptionCommand()
{
}

bool SetSystemDescriptionCommand::doExecute()
{
    _system->setDescription( _description );
    return true;
}

// SetSystemRootCommand //

SetSystemRootCommand::SetSystemRootCommand( coca::ISystem& system, const std::string& componentId,
        const std::string& name ) :
        _system( &system ),
        _node( 0 ),
        _componentId( componentId ),
        _name( name )
{
}

SetSystemRootCommand::SetSystemRootCommand( coca::ISystem& system, coca::INode& node ) :
        _system( &system ),
        _node( &node )
{
}

SetSystemRootCommand::~SetSystemRootCommand()
{
}

bool SetSystemRootCommand::doExecute()
{
    coca::NodePtr node;
    if ( _node )
    {
        node = _node;
    }
    else if ( _componentId.empty() )
    {
        node = coca::getFactory().createCompositeNode( _name );
    }
    else
    {
        node = coca::getFactory().createComponentNode( _componentId, _name );
    }
    if ( node )
    {
        coca::NodePtr oldRoot( _system->getRoot() );
        _system->setRoot( node );
        if ( oldRoot ) { COCA_CHECK_EQUAL_SILENT( oldRoot->getReferenceCount(), 1 ); }
    }
    return ( node != 0 );
}

// ClearSystemRootCommand //

ClearSystemRootCommand::ClearSystemRootCommand( coca::ISystem& system ) :
        _system( &system )
{
}

ClearSystemRootCommand::~ClearSystemRootCommand()
{
}

bool ClearSystemRootCommand::doExecute()
{
    coca::NodePtr oldRoot( _system->getRoot() );
    _system->setRoot( 0 );
    if ( oldRoot ) { COCA_CHECK_EQUAL_SILENT( oldRoot->getReferenceCount(), 1 ); }
    return true;
}

// SetNodeNameCommand //

SetNodeNameCommand::SetNodeNameCommand( coca::INode& node, const std::string& name ) :
        _node( &node ),
        _name( name )
{
}
SetNodeNameCommand::~SetNodeNameCommand()
{
}

bool SetNodeNameCommand::doExecute()
{
    _node->setName( _name );
    return true;
}

// EnableNodeCommand //

EnableNodeCommand::EnableNodeCommand( coca::INode& node, bool enableChildren ) :
        _node( &node ),
        _enableChildren( enableChildren )
{
}
EnableNodeCommand::~EnableNodeCommand()
{
}

bool EnableNodeCommand::doExecute()
{
    _node->enable( _enableChildren );
    return true;
}

// DisableNodeCommand //

DisableNodeCommand::DisableNodeCommand( coca::INode& node, bool disableChildren ) :
        _node( &node ),
        _disableChildren( disableChildren )
{
}
DisableNodeCommand::~DisableNodeCommand()
{
}

bool DisableNodeCommand::doExecute()
{
    _node->disable( _disableChildren );
    return true;
}

// AppendNodeCommand //

AppendNodeCommand::AppendNodeCommand( coca::INode& parent, const std::string& componentId,
                                      const std::string& name ) :
        _parent( &parent ),
        _node( 0 ),
        _componentId( componentId ),
        _name( name )
{
}

AppendNodeCommand::AppendNodeCommand( coca::INode& parent, coca::INode& node ) :
        _parent( &parent ),
        _node( &node )
{
}

AppendNodeCommand::~AppendNodeCommand()
{
}

bool AppendNodeCommand::doExecute()
{
    if ( _node )
    {
        return _parent->append( _node );
    }
    else
    {
        coca::NodePtr node;
        if ( _componentId.empty() )
        {
            node = coca::getFactory().createCompositeNode( _name, _parent );
        }
        else
        {
            node = coca::getFactory().createComponentNode( _componentId, _name, _parent );
        }
        return ( node != 0 && node->getParent() == _parent );
    }
}

// RemoveNodeCommand //

RemoveNodeCommand::RemoveNodeCommand( coca::INode& node ) :
        _node( &node )
{
}

RemoveNodeCommand::~RemoveNodeCommand()
{
}

bool RemoveNodeCommand::doExecute()
{
    coca::NodePtr node( _node );
    bool ok = false;
    if ( _node->getParent() )
    {
        ok = _node->getParent()->remove( _node );
    }
    COCA_CHECK_EQUAL_SILENT( node->getReferenceCount(), 1 );
    return ok;
}

// MoveNodeCommand //

MoveNodeCommand::MoveNodeCommand( coca::INode& node, coca::INode& newParent ) :
        _node( &node ),
        _newParent( &newParent )
{
}

MoveNodeCommand::~MoveNodeCommand()
{
}

bool MoveNodeCommand::doExecute()
{
    return _newParent->append( _node );
}

// MoveNodeUpCommand //

MoveNodeUpCommand::MoveNodeUpCommand( coca::INode& node ) :
        _node( &node )
{
}

MoveNodeUpCommand::~MoveNodeUpCommand()
{
}

bool MoveNodeUpCommand::doExecute()
{
    if ( _node->getParent() ) { return _node->getParent()->moveUp( _node ); }
    return false;
}

// MoveNodeDownCommand //

MoveNodeDownCommand::MoveNodeDownCommand( coca::INode& node ) :
        _node( &node )
{
}

MoveNodeDownCommand::~MoveNodeDownCommand()
{
}

bool MoveNodeDownCommand::doExecute()
{
    if ( _node->getParent() ) { return _node->getParent()->moveDown( _node ); }
    return false;
}

// ConnectAttributesCommand //

ConnectAttributesCommand::ConnectAttributesCommand( coca::IAttribute& sourceAttribute, coca::IAttribute& sinkAttribute ) :
        _sourceAttribute( &sourceAttribute ),
        _sinkAttribute( &sinkAttribute )
{
}

ConnectAttributesCommand::~ConnectAttributesCommand()
{
}

bool ConnectAttributesCommand::doExecute()
{
    return _sourceAttribute->connectSink( *_sinkAttribute );
}

// DisconnectSourceCommand //

DisconnectSourceCommand::DisconnectSourceCommand( coca::IAttribute& attribute, coca::IAttribute& sourceAttribute ) :
        _attribute( &attribute ),
        _sourceAttribute( &sourceAttribute )
{
}

DisconnectSourceCommand::~DisconnectSourceCommand()
{
}

bool DisconnectSourceCommand::doExecute()
{
    return _attribute->disconnectSource( *_sourceAttribute );
}

// SetAttributeValueCommand //

SetAttributeValueCommand::SetAttributeValueCommand( coca::IAttribute& attribute, const std::string& valueString ) :
        _attribute( &attribute ),
        _valueString( valueString )
{
}

SetAttributeValueCommand::~SetAttributeValueCommand()
{
}

bool SetAttributeValueCommand::doExecute()
{
    return _attribute->setAsString( _valueString );
}

