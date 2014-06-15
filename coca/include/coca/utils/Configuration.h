// Copyright (C) 2008-2009 Harm Boschloo

#ifndef COCA_CONFIGURATION_H_INCLUDED
#define COCA_CONFIGURATION_H_INCLUDED

#include "IConfiguration.h"
#include "config.h"

namespace coca
{
    class Configuration : public IConfiguration
    {
    public:
        Configuration( EPlatformType platform = COCA_PLATFORM, ECompilerType compiler = COCA_COMPILER ) :
                _platform( platform ),
                _compiler( compiler )
        {
        }

        void setPlatform( EPlatformType platform ) { _platform = platform; }
        EPlatformType getPlatform() const { return _platform; }

        void setCompiler( ECompilerType compiler ) { _compiler = compiler; }
        ECompilerType getCompiler() const { return _compiler; }

        bool operator==( const IConfiguration& other ) const
        {
            return ( _platform == other.getPlatform() && _compiler == other.getCompiler() );
        }

        bool operator!=( const IConfiguration& other ) const { return !( operator==( other ) ); }

    private:
        EPlatformType _platform;
        ECompilerType _compiler;
    };

} // namespace coca

#endif //COCA_CONFIGURATION_H_INCLUDED
