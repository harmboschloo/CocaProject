// Copyright (C) 2007-2008 Harm Boschloo

#ifndef A_COMMAND_H_INCLUDED
#define A_COMMAND_H_INCLUDED

#include <coca/utils/ReferenceCounted.h>

class ACommand : public coca::ReferenceCounted
{
public:
    ACommand();
    virtual ~ACommand();

    bool execute();
    bool hadError() const;

protected:
    virtual bool doExecute() = 0;

private:
    bool _hadError;
};

#endif // A_COMMAND_H_INCLUDED

