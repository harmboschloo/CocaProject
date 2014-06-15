// Copyright (C) 2007-2008 Harm Boschloo

#ifndef COCA_RESOURCE_MANAGER_H_INCLUDED
#define COCA_RESOURCE_MANAGER_H_INCLUDED

#include "CocaLoggerSentry.h"
#include "CommandPtr.h"
#include "CocaThread.h"
#include <wx/thread.h>
#include <memory>
#include <deque>

class ICommand;

class CocaResourceManager
{
public:
    CocaResourceManager();
    ~CocaResourceManager();

    void createThread();
    void destroyThread();

    void postCommand( const CommandPtr& command );
    void executeCommands();
    bool executeCommand( const CommandPtr& command );

private:
    friend class CocaThread;

    void processCommands();
    wxCriticalSection& getCriticalSection();
    CommandPtr getNextCommand();

    CocaLoggerSentry _logSentry;
    int _commandsLimit;
    wxCriticalSection _cs;
    wxCriticalSectionLocker _lock;
    CocaThread _thread;
    std::deque<CommandPtr> _commands;
};

#endif // COCA_RESOURCE_MANAGER_H_INCLUDED

