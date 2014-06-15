// Copyright (C) 2007-2008 Harm Boschloo

#ifndef WX_CRITICAL_SECTION_UNLOCKER_H_INCLUDED
#define WX_CRITICAL_SECTION_UNLOCKER_H_INCLUDED

class wxCriticalSection;

class WXCriticalSectionUnlocker
{
public:
    WXCriticalSectionUnlocker( wxCriticalSection& cs );
    ~WXCriticalSectionUnlocker();

private:
    wxCriticalSection* _cs;
};

#endif // WX_CRITICAL_SECTION_UNLOCKER_H_INCLUDED

