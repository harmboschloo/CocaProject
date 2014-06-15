// Copyright (C) 2007-2008 Harm Boschloo

#include "WXMutexUnlocker.h"
#include <wx/thread.h>
#include <coca/utils/test.h>

WXMutexUnlocker::WXMutexUnlocker( wxMutex& mutex ) : _mutex( &mutex )
{
    COCA_CHECK_EQUAL_SILENT( _mutex->Unlock(), wxMUTEX_NO_ERROR );
}

WXMutexUnlocker::~WXMutexUnlocker()
{
    COCA_CHECK_EQUAL_SILENT( _mutex->Lock(), wxMUTEX_NO_ERROR );
}
