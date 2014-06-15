// Copyright (C) 2008-2009 Harm Boschloo

#ifndef COCA_NON_COPYABLE_H_INCLUDED
#define COCA_NON_COPYABLE_H_INCLUDED

namespace coca
{
    class NonCopyable
    {
    protected:
        NonCopyable() {};
        ~NonCopyable() {};

    private:
        NonCopyable( const NonCopyable& );
        NonCopyable& operator=( const NonCopyable& );
    };

} // namespace coca

#endif // COCA_NON_COPYABLE_H_INCLUDED
