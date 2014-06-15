// Copyright (C) 2007-2008 Harm Boschloo

#ifndef COCA_COMMANDS_H_INCLUDED
#define COCA_COMMANDS_H_INCLUDED

#include "ACommand.h"
#include "CommandPtr.h"
#include <coca/ISystem.h>
#include <coca/INode.h>
#include <coca/IReader.h>
#include <string>

// CompositeCommand //

class CompositeCommand: public ACommand
{
public:
    bool doExecute();
    void append( const CommandPtr& command );

private:
    std::vector<CommandPtr> _commands;
};


// ClockTickCommand //

class ClockTickCommand: public ACommand
{
public:
    bool doExecute();
};

// ResetClockCommand //

class ResetClockCommand: public ACommand
{
public:
    bool doExecute();
};

// CreateSystemCommand //

class CreateSystemCommand: public ACommand
{
public:
    CreateSystemCommand( coca::ISystem** system );
    ~CreateSystemCommand();

    bool doExecute();

private:
    coca::ISystem** _system;
};

// DeleteSystemCommand //

class DeleteSystemCommand: public ACommand
{
public:
    DeleteSystemCommand( coca::ISystem* system );
    ~DeleteSystemCommand();

    bool doExecute();

private:
    coca::ISystem* _system;
};

// SaveSystemCommand //

class SaveSystemCommand: public ACommand
{
public:
    SaveSystemCommand( const coca::ISystem& system, const std::string& filename );
    ~SaveSystemCommand();

    bool doExecute();

private:
    const coca::ISystem* _system;
    const std::string _filename;
};

// LoadSystemCommand //

class LoadSystemCommand: public ACommand
{
public:
    LoadSystemCommand( coca::ISystem** system, const std::string& filename );
    ~LoadSystemCommand();

    bool doExecute();

private:
    coca::ISystem** _system;
    const std::string _filename;
};

// ReadNodeCommand //

class ReadNodeCommand: public ACommand
{
public:
    ReadNodeCommand( const std::string& text, coca::IReader& reader, coca::INode** node );
    ~ReadNodeCommand();

    bool doExecute();

private:
    std::string _text;
    coca::IReader* _reader;
    coca::INode** _node;
};

// SetSystemDescriptionCommand //

class SetSystemDescriptionCommand: public ACommand
{
public:
    SetSystemDescriptionCommand( coca::ISystem& system, const std::string& description );
    ~SetSystemDescriptionCommand();

    bool doExecute();

private:
    coca::ISystem* _system;
    const std::string _description;
};

// SetSystemRootCommand //

class SetSystemRootCommand: public ACommand
{
public:
    SetSystemRootCommand( coca::ISystem& system, const std::string& componentId, const std::string& name );
    SetSystemRootCommand( coca::ISystem& system, coca::INode& node );
    ~SetSystemRootCommand();

    bool doExecute();

private:
    coca::ISystem* _system;
    coca::INode* _node;
    const std::string _componentId;
    const std::string _name;
};

// ClearSystemRootCommand //

class ClearSystemRootCommand: public ACommand
{
public:
    ClearSystemRootCommand( coca::ISystem& system );
    ~ClearSystemRootCommand();

    bool doExecute();

private:
    coca::ISystem* _system;
};

// SetNodeNameCommand //

class SetNodeNameCommand: public ACommand
{
public:
    SetNodeNameCommand( coca::INode& node, const std::string& name );
    ~SetNodeNameCommand();

    bool doExecute();

private:
    coca::INode* _node;
    std::string _name;
};

// EnableNodeCommand //

class EnableNodeCommand: public ACommand
{
public:
    EnableNodeCommand( coca::INode& node, bool enableChildren );
    ~EnableNodeCommand();

    bool doExecute();

private:
    coca::INode* _node;
    bool _enableChildren;
};


// DisableNodeCommand //

class DisableNodeCommand: public ACommand
{
public:
    DisableNodeCommand( coca::INode& node, bool disableChildren );
    ~DisableNodeCommand();

    bool doExecute();

private:
    coca::INode* _node;
    bool _disableChildren;
};

// AppendNodeCommand //

class AppendNodeCommand: public ACommand
{
public:
    AppendNodeCommand( coca::INode& parent, const std::string& componentId, const std::string& name );
    AppendNodeCommand( coca::INode& parent, coca::INode& node );
    ~AppendNodeCommand();

    bool doExecute();

private:
    coca::INode* _parent;
    coca::INode* _node;
    std::string _componentId;
    std::string _name;
};

// RemoveNodeCommand //

class RemoveNodeCommand: public ACommand
{
public:
    RemoveNodeCommand( coca::INode& node );
    ~RemoveNodeCommand();

    bool doExecute();

private:
    coca::INode* _node;
};

// MoveNodeCommand //

class MoveNodeCommand: public ACommand
{
public:
    MoveNodeCommand( coca::INode& node, coca::INode& newParent );
    ~MoveNodeCommand();

    bool doExecute();

private:
    coca::INode* _node;
    coca::INode* _newParent;
};

// MoveNodeUpCommand //

class MoveNodeUpCommand: public ACommand
{
public:
    MoveNodeUpCommand( coca::INode& node );
    ~MoveNodeUpCommand();

    bool doExecute();

private:
    coca::INode* _node;
};

// MoveNodeDownCommand //

class MoveNodeDownCommand: public ACommand
{
public:
    MoveNodeDownCommand( coca::INode& node );
    ~MoveNodeDownCommand();

    bool doExecute();

private:
    coca::INode* _node;
};

// ConnectAttributesCommand //

class ConnectAttributesCommand: public ACommand
{
public:
    ConnectAttributesCommand( coca::IAttribute& sourceAttribute, coca::IAttribute& sinkAttribute );
    ~ConnectAttributesCommand();

    bool doExecute();

private:
    coca::IAttribute* _sourceAttribute;
    coca::IAttribute* _sinkAttribute;
};

// DisconnectSourceCommand //

class DisconnectSourceCommand: public ACommand
{
public:
    DisconnectSourceCommand( coca::IAttribute& attribute, coca::IAttribute& sourceAttribute );
    ~DisconnectSourceCommand();

    bool doExecute();

private:
    coca::IAttribute* _attribute;
    coca::IAttribute* _sourceAttribute;
};

// SetAttributeValueCommand //

class SetAttributeValueCommand: public ACommand
{
public:
    SetAttributeValueCommand( coca::IAttribute& attribute, const std::string& valueString );
    ~SetAttributeValueCommand();

    bool doExecute();

private:
    coca::IAttribute* _attribute;
    std::string _valueString;
};


#endif // COCA_DOCUMENT_H_INCLUDED

