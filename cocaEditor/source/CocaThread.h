// Copyright (C) 2007-2008 Harm Boschloo

#ifndef COCA_THREAD_H_INCLUDED
#define COCA_THREAD_H_INCLUDED

#include <wx/thread.h>

class CocaResourceManager;

class CocaThread : public wxThread
{
public:
    CocaThread( CocaResourceManager& manager );
    ~CocaThread();

    void processCommands();
    void stop();

protected:
    ExitCode Entry();

    bool _stop;
    CocaResourceManager* _manager;
};

#endif // COCA_THREAD_H_INCLUDED

