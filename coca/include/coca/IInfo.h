// Copyright (C) 2008-2009 Harm Boschloo

#ifndef COCA_I_INFO_H_INCLUDED
#define COCA_I_INFO_H_INCLUDED

#include "interface.h"

namespace coca
{
    class IConfiguration;
    class IVersion;

    class IInfo
    {
    public:
        virtual const IConfiguration& getConfiguration() const = 0;
        virtual const IVersion& getVersion() const = 0;
        virtual const char* getVersionName() const = 0;
        virtual const char* getBuildDateTime() const = 0;

    protected:
        virtual ~IInfo() {}
    };

} // namespace coca

#endif //COCA_I_INFO_H_INCLUDED
