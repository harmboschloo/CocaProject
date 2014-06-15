// Copyright (C) 2007-2008 Harm Boschloo

#include "CocaThread.h"
#include "CocaResourceManager.h"
#include "ACommand.h"
#include <coca/log/log.h>

CocaThread::CocaThread( CocaResourceManager& manager ) :
        wxThread( wxTHREAD_JOINABLE ),
        _stop( false ),
        _manager( &manager )
{
    COCA_DEBUG_INFO( "CocaThread Created" );
}

CocaThread::~CocaThread()
{
    COCA_DEBUG_INFO( "CocaThread Destroyed" );
}

void CocaThread::processCommands()
{
    CommandPtr command = _manager->getNextCommand();
    while ( command )
    {
        command->execute();
        command = _manager->getNextCommand();
    }
}

void CocaThread::stop()
{
    _stop = true;
}

wxThread::ExitCode CocaThread::Entry()
{
    COCA_DEBUG_INFO( "CocaThread::Entry" );

    // somehow TestDestroy does not work properly here...
    while ( !TestDestroy() )
    {
        {
            //COCA_DEBUG_INFO( "waiting for lock" );
            wxCriticalSectionLocker lock( _manager->getCriticalSection() );
            //COCA_DEBUG_INFO( "locked, processing commands" );
            processCommands();
            //COCA_DEBUG_INFO( "commands processed" );
            if ( _stop ) { break; }
            //COCA_DEBUG_INFO( "unlocking" );
        }
        //COCA_DEBUG_INFO( "unlocked" );
    }

    COCA_DEBUG_INFO( "end CocaThread::Entry" );

    return 0;
}

