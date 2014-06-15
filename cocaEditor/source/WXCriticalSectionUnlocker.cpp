// Copyright (C) 2007-2008 Harm Boschloo

#include "WXCriticalSectionUnlocker.h"
#include <wx/thread.h>

WXCriticalSectionUnlocker::WXCriticalSectionUnlocker( wxCriticalSection& cs ) : _cs( &cs )
{
    _cs->Leave();
}

WXCriticalSectionUnlocker::~WXCriticalSectionUnlocker()
{
    _cs->Enter();
}
