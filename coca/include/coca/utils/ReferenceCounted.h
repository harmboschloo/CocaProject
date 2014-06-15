// Copyright (C) 2008-2009 Harm Boschloo

#ifndef COCA_REFERENCE_COUNTED_H_INCLUDED
#define COCA_REFERENCE_COUNTED_H_INCLUDED

#include "NonCopyable.h"

namespace coca
{
    class ReferenceCounted : private NonCopyable
    {
    public:
        ReferenceCounted() : _referenceCount( 0 )
        {
        }

        void grab() const
        {
            ++_referenceCount;
        }

        bool drop() const
        {
            --_referenceCount;
            if ( _referenceCount <= 0 )
            {
                delete this;
                return true;
            }
            return false;
        }

        int getReferenceCount() const
        {
            return _referenceCount;
        }

    protected:
        virtual ~ReferenceCounted()
        {
        }

    private:
        mutable int _referenceCount;
    };

} // namespace coca

#endif //COCA_REFERENCE_COUNTED_H_INCLUDED
