// Copyright (C) 2008-2009 Harm Boschloo

#ifndef COCA_I_PLUGIN_INFO_H_INCLUDED
#define COCA_I_PLUGIN_INFO_H_INCLUDED

namespace coca
{
    class IConfiguration;
    class IVersion;

    class IPluginInfo
    {
    public:
        virtual ~IPluginInfo() {}

        virtual const IVersion& getVersion() const = 0;
        virtual const char* getName() const = 0;
        virtual const char* getAuthor() const = 0;
        virtual const char* getEmail() const = 0;
        virtual const char* getWebsite() const = 0;
        virtual const char* getDescription() const = 0;
        virtual const IConfiguration& getConfiguration() const = 0;
        virtual const char* getBuildDataTime() const = 0;
        virtual const IVersion& getCocaVersion() const = 0;
    };

} // namespace coca

#endif // COCA_I_PLUGIN_INFO_H_INCLUDED
