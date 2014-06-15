// Copyright (C) 2008-2009 Harm Boschloo

#ifndef COCA_I_CONFIGURATION_H_INCLUDED
#define COCA_I_CONFIGURATION_H_INCLUDED

#include "EPlatformType.h"
#include "ECompilerType.h"

namespace coca
{
    class IConfiguration
    {
    public:
        virtual ~IConfiguration() {}

        virtual void setPlatform( EPlatformType platform ) = 0;
        virtual EPlatformType getPlatform() const = 0;

        virtual void setCompiler( ECompilerType compiler ) = 0;
        virtual ECompilerType getCompiler() const = 0;

        virtual bool operator==( const IConfiguration& other ) const = 0;
        virtual bool operator!=( const IConfiguration& other ) const = 0;
    };

} // namespace coca

#endif //COCA_I_CONFIGURATION_H_INCLUDED
