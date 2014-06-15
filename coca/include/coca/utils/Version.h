// Copyright (C) 2008-2009 Harm Boschloo

#ifndef COCA_UTILS_VERSION_H_INCLUDED
#define COCA_UTILS_VERSION_H_INCLUDED

#include "IVersion.h"

namespace coca
{
    class Version : public IVersion
    {
    public:
        Version( int major = 0, int minor = 0, int revision = 0, const char* status = "" ) :
                _major( major ),
                _minor( minor ),
                _revision( revision ),
                _status( status )
        {
        }

        void set( int major, int minor, int revision, const char* status )
        {
            _major = major;
            _minor = minor;
            _revision = revision;
            _status = status;
        }

        int getMajor() const { return _major; }
        int getMinor() const { return _minor; }
        int getRevision() const { return _revision; }
        const char* getStatus() const { return _status; }

    private:
        int _major;
        int _minor;
        int _revision;
        const char* _status;
    };

} // namespace coca

#endif //COCA_UTILS_VERSION_H_INCLUDED
