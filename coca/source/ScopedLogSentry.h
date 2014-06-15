// Copyright (C) 2008-2009 Harm Boschloo

#ifndef COCA_SCOPED_SENTRY_H_INCLUDED
#define COCA_SCOPED_SENTRY_H_INCLUDED

namespace coca
{
    class ILoggerSentry;

    class ScopedLogSentry
    {
    public:
        ScopedLogSentry( ILoggerSentry* sentry );
        ~ScopedLogSentry();

    private:
        ILoggerSentry* _sentry;
    };

} // namespace coca

#endif // COCA_SCOPED_SENTRY_H_INCLUDED

