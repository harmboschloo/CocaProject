// Copyright (C) 2008-2009 Harm Boschloo

#include "Clock.h"
#include <coca/utils/algorithm/eraseItem.h>
#include <coca/utils/algorithm/callAll.h>

namespace coca
{

    Clock::Clock() :
            _stop( false ),
            _tickClock( 0 ),
            _frameRateRecorder( 1 )
    {
    }

    Clock::~Clock()
    {
    }

    void Clock::stop()
    {
        _stop = true;
    }

    bool Clock::stopped() const
    {
        return _stop;
    }

    void Clock::reset()
    {
        _stop = false;
        _frameRateRecorder.resetRecording();
    }

    void Clock::tick()
    {
        _tickClock = clock();
        _frameRateRecorder.recordFrame();
        callAllRef( _callbacks );
    }

    clock_t Clock::getTickClock() const
    {
        return _tickClock;
    }

    float Clock::getTickRate() const
    {
        return _frameRateRecorder.getFrameRate();
    }

    void Clock::appendTickCallback( const Function<>& function )
    {
        _callbacks.push_back( function );
    }

    void Clock::removeTickCallback( const Function<>& function )
    {
        eraseItem( _callbacks, function );
    }

} // namespace coca
