// Copyright (C) 2008-2009 Harm Boschloo

#ifndef COCA_I_CLOCK_H_INCLUDED
#define COCA_I_CLOCK_H_INCLUDED

#include "interface.h"
#include "utils/function/autogen/Function0.h"
#include <ctime>

namespace coca
{
    class IClock
    {
    public:
        virtual void stop() = 0;
        virtual bool stopped() const = 0;
        virtual void reset() = 0;
        virtual void tick() = 0;
        virtual clock_t getTickClock() const = 0;
        virtual float getTickRate() const = 0;
        virtual void appendTickCallback( const Function<>& function ) = 0;
        virtual void removeTickCallback( const Function<>& function ) = 0;

    protected:
        virtual ~IClock() {}
    };

} // namespace coca

#endif // COCA_I_CLOCK_H_INCLUDED
