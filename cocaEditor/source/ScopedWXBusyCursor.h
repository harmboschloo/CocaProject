// Copyright (C) 2007-2008 Harm Boschloo

#ifndef SCOPED_WX_BUSY_CURSOR_H_INCLUDED
#define SCOPED_WX_BUSY_CURSOR_H_INCLUDED

#include <wx/utils.h>

class ScopedWXBusyCursor
{
public:
    ScopedWXBusyCursor()
    {
        wxBeginBusyCursor();
    }

    ~ScopedWXBusyCursor()
    {
        wxEndBusyCursor();
    }
};

#endif // SCOPED_WX_BUSY_CURSOR_H_INCLUDED


