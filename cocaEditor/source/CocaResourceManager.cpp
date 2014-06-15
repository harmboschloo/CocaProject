// Copyright (C) 2007-2008 Harm Boschloo

#include "CocaResourceManager.h"
#include "ACommand.h"
#include "WXCriticalSectionUnlocker.h"
#include <coca/log/log.h>
#include <wx/utils.h>

CocaResourceManager::CocaResourceManager() :
    _commandsLimit( -1 ),
    _lock( _cs ), // critical section is locked by default, see processCommands
    _thread( *this )
{
    coca::getLogger().setSentry( &_logSentry );
    COCA_DEBUG_INFO( "CocaResourceManager Created" );
}

CocaResourceManager::~CocaResourceManager()
{
    coca::getLogger().setSentry( 0 );
    COCA_DEBUG_INFO( "CocaResourceManager Destroyed" );
}

void CocaResourceManager::createThread()
{
    COCA_DEBUG_INFO( "CocaResourceManager creating thread" );

    COCA_REQUIRE_EQUAL_SILENT( _thread.Create(), wxTHREAD_NO_ERROR );
    COCA_REQUIRE_EQUAL_SILENT( _thread.Run(), wxTHREAD_NO_ERROR );

    COCA_DEBUG_INFO( "CocaResourceManager created thread" );
}

void CocaResourceManager::destroyThread()
{
    COCA_DEBUG_INFO( "CocaResourceManager destroying thread" );

    _thread.stop(); // TODO: somehow Delete/Wait does not work... maybe due to the lock...?
    WXCriticalSectionUnlocker enableThread( _cs );
    COCA_CHECK_NULL( _thread.Wait() );

    COCA_DEBUG_INFO( "CocaResourceManager destroyed thread" );
}

void CocaResourceManager::postCommand( const CommandPtr& command )
{
    _commands.push_back( command );
}

void CocaResourceManager::executeCommands()
{
    if ( _commands.empty() ) { return; }

    _commandsLimit = -1;
   processCommands();

    COCA_CHECK_SILENT( _commands.empty() );
}

bool CocaResourceManager::executeCommand( const CommandPtr& command )
{
     if ( !command ) { return false; }

    _commandsLimit = 1;
    _commands.push_front( command );
    processCommands();

    if ( !_commands.empty() )
    {
        COCA_CHECK_SILENT( _commands.front() != command );
    }

    return !command->hadError();
}

void CocaResourceManager::processCommands()
{
    //COCA_DEBUG_INFO( "CocaResourceManager::processCommands " << _commands.size() << "; " << _commandsLimit );

    // this lets the thead in to process the commands
    while( _commandsLimit != 0 && !_commands.empty() )
    {
        //_thread.processCommands(); continue; // skip thread, and process directly

        {
            //COCA_DEBUG_INFO( "CocaResourceManager unlocking" );
            WXCriticalSectionUnlocker enableThread( _cs );
            //COCA_DEBUG_INFO( "CocaResourceManager unlocked" );
            wxMilliSleep( 0 );
            //COCA_DEBUG_INFO( "CocaResourceManager locking" );
        }
        //COCA_DEBUG_INFO( "CocaResourceManager locked" );
    }
}

// private //

wxCriticalSection& CocaResourceManager::getCriticalSection()
{
    return _cs;
}

CommandPtr CocaResourceManager::getNextCommand()
{
    if ( _commandsLimit == 0 || _commands.empty() ) { return CommandPtr(); }

    if ( _commandsLimit > 0 ) { --_commandsLimit; }

    CommandPtr command = _commands.front();
    _commands.pop_front();
    return command;
}

