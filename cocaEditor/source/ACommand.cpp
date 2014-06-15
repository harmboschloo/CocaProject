// Copyright (C) 2007-2008 Harm Boschloo

#include "ACommand.h"

ACommand::ACommand() :
        _hadError( false )
{
}

ACommand::~ACommand()
{
}

bool ACommand::execute()
{
    _hadError = !doExecute();
    return !_hadError;
}

bool ACommand::hadError() const
{
    return _hadError;
}
