// Copyright (C) 2007-2008 Harm Boschloo

#ifndef WX_MUTEX_UNLOCKER_H_INCLUDED
#define WX_MUTEX_UNLOCKER_H_INCLUDED

class wxMutex;

class WXMutexUnlocker
{
public:
    WXMutexUnlocker( wxMutex& mutex );
    ~WXMutexUnlocker();

private:
    wxMutex* _mutex;
};

#endif // WX_MUTEX_UNLOCKER_H_INCLUDED

