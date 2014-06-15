// Copyright (C) 2008-2009 Harm Boschloo

#include "Info.h"
#include <coca/version.h>

namespace coca
{
    Info::Info() :
        _configuration(),
        _version( COCA_VERSION )
    {
    }

    Info::~Info()
    {
    }

    const IConfiguration& Info::getConfiguration() const
    {
        return _configuration;
    }

    const IVersion& Info::getVersion() const
    {
        return _version;
    }

    const char* Info::getVersionName() const
    {
        return COCA_VERSION_NAME;
    }

    const char* Info::getBuildDateTime() const
    {
        return COCA_DATE_TIME;
    }

} // namespace coca

