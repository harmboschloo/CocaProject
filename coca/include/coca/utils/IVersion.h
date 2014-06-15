// Copyright (C) 2008-2009 Harm Boschloo

#ifndef COCA_I_VERSION_H_INCLUDED
#define COCA_I_VERSION_H_INCLUDED

#include <ostream>

namespace coca
{
    class IVersion
    {
    public:
        virtual ~IVersion() {}
        virtual void set( int major, int minor, int revision, const char* status ) = 0;
        virtual int getMajor() const = 0;
        virtual int getMinor() const = 0;
        virtual int getRevision() const = 0;
        virtual const char* getStatus() const = 0;
    };

} // namespace coca

inline std::ostream& operator<<( std::ostream& stream, const coca::IVersion& version )
{
    return stream << version.getMajor() << "." << version.getMinor() << "."
           << version.getRevision() << " " << version.getStatus();
}

#endif //COCA_I_VERSION_H_INCLUDED
