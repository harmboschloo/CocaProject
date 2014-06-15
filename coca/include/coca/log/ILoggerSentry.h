// Copyright (C) 2008-2009 Harm Boschloo

#ifndef COCA_I_LOGGER_SENTRY_H_INCLUDED
#define COCA_I_LOGGER_SENTRY_H_INCLUDED

namespace coca
{
    class ILoggerSentry
    {
    public:
        virtual ~ILoggerSentry() {};

        virtual void enter() = 0;
        virtual void leave() = 0;
    };

} // namespace coca

#endif // COCA_I_LOGGER_SENTRY_H_INCLUDED

