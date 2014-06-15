// Copyright (C) 2008-2009 Harm Boschloo

#ifndef COCA_INFO_H_INCLUDED
#define COCA_INFO_H_INCLUDED

#include <coca/IInfo.h>
#include <coca/utils/Configuration.h>
#include <coca/utils/Version.h>

namespace coca
{
    class Info : public IInfo
    {
    public:
        Info();
        ~Info();

        const IConfiguration& getConfiguration() const;
        const IVersion& getVersion() const;
        const char* getVersionName() const;
        const char* getBuildDateTime() const;

    private:
        Configuration _configuration;
        Version _version;
    };

} // namespace coca

#endif //COCA_INFO_H_INCLUDED
