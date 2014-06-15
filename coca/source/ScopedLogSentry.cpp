// Copyright (C) 2008-2009 Harm Boschloo

#include "ScopedLogSentry.h"
#include <coca/log/ILoggerSentry.h>

namespace coca
{
    ScopedLogSentry::ScopedLogSentry( ILoggerSentry* sentry ) :
        _sentry( sentry )
    {
        if ( _sentry ) { _sentry->enter(); }
    }

    ScopedLogSentry::~ScopedLogSentry()
    {
        if ( _sentry ) { _sentry->leave(); }
    }

} // namespace coca
