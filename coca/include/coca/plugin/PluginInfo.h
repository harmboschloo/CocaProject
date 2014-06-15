// Copyright (C) 2008-2009 Harm Boschloo

#ifndef COCA_PLUGIN_INFO_H_INCLUDED
#define COCA_PLUGIN_INFO_H_INCLUDED

#include "IPluginInfo.h"
#include "../version.h"
#include "../utils/Configuration.h"
#include "../utils/Version.h"
#include "../utils/NonCopyable.h"
#include "../utils/preprocessor.h"

namespace coca
{
    class PluginInfo : public IPluginInfo, private NonCopyable
    {
    public:
        PluginInfo( const char* name = "", const char* author = "", const char* email = "",
                    const char* website = "", const char* description = "" ) :
                _name( name ),
                _author( author ),
                _email( email ),
                _website( website ),
                _description( description ),
                _buildDateTime( COCA_DATE_TIME ),
                _cocaVersion( COCA_VERSION )
        {
        }

		~PluginInfo() 
		{
		}

        Version& getVersion() { return _version; }
        const IVersion& getVersion() const { return _version; }

        const char* getName() const { return _name; }
        void setName( const char* name ) { _name = name; }

        const char* getAuthor() const { return _author; }
        void setAuthor( const char* author ) { _author = author; }

        const char* getEmail() const { return _email; }
        void setEmail( const char* email ) { _email = email; }

        const char* getWebsite() const { return _website; }
        void setWebsite( const char* website ) { _website = website; }

        const char* getDescription() const { return _description; }
        void setDescription( const char* description ) { _description = description; }

        const IConfiguration& getConfiguration() const { return _configuration; }
        const char* getBuildDataTime() const { return _buildDateTime; }
        const IVersion& getCocaVersion() const { return _cocaVersion; }

    private:
        Version _version;
        const char* _name;
        const char* _author;
        const char* _email;
        const char* _website;
        const char* _description;
        const Configuration _configuration;
        const char* _buildDateTime;
        const Version _cocaVersion;
    };

} // namespace coca

#endif // COCA_PLUGIN_INFO_H_INCLUDED
