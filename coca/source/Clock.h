// Copyright (C) 2008-2009 Harm Boschloo

#ifndef COCA_CLOCK_H_INCLUDED
#define COCA_CLOCK_H_INCLUDED

#include <coca/IClock.h>
#include <coca/utils/FrameRateRecorder.h>
#include <vector>

namespace coca
{
    class Clock : public IClock
    {
    public:
        Clock();
        ~Clock();

        void stop();
        bool stopped() const;
        void reset();
        void tick();
        clock_t getTickClock() const;
        float getTickRate() const;
        void appendTickCallback( const Function<>& function );
        void removeTickCallback( const Function<>& function );

    private:
        typedef std::vector<Function<> > FunctionVector;

        bool _stop;
        clock_t _tickClock;
        FrameRateRecorder<float> _frameRateRecorder;
        FunctionVector _callbacks;
    };

} // namespace coca

#endif // COCA_CLOCK_H_INCLUDED
