// Copyright (C) 2007-2008 Harm Boschloo

#ifndef COCA_LOGGER_SENTRY_H_INCLUDED
#define COCA_LOGGER_SENTRY_H_INCLUDED

#include <coca/log/ILoggerSentry.h>
#include <wx/thread.h>

class CocaLoggerSentry : public coca::ILoggerSentry
{
public:
    CocaLoggerSentry();
    ~CocaLoggerSentry();

    void enter();
    void leave();

private:
    wxCriticalSection _cs;
};

#endif // COCA_LOGGER_SENTRY_H_INCLUDED

