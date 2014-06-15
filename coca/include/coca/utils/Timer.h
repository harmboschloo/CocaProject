// Copyright (C) 2008-2009 Harm Boschloo

#ifndef COCA_TIMER_H_INCLUDED
#define COCA_TIMER_H_INCLUDED

#include <ctime>

namespace coca
{
 template<typename T>
    class Timer
    {
    public:
        Timer( T targetTimeDelay = 0 ) :
            _timeDelay( targetTimeDelay ),
            _runTime( 0 ),
            _targetTime( 0 )
        {
        }

        ~Timer()
        {
        }

        T getTargetTimeDelay() const
        {
            return _timeDelay;
        }

        T getRunTime() const
        {
            return _runTime;
        }

        T getTargetTime() const
        {
            return _targetTime;
        }

        void resetTargetTimeDelay( T timeDelay = 0 )
        {
            _targetTime = 0;
            _timeDelay = timeDelay;
        }

        void reset()
        {
            _targetTime = 0;
        }

        /// updates the timer and returns the time (in seconds) untill the target time (can be negative)
        T run()
        {
            _runTime = clock()/static_cast<T>( CLOCKS_PER_SEC );

            if ( _timeDelay <= 0 ) { return 0; }

            // init next clock
            if ( _targetTime == 0 )
            {
                _targetTime = _runTime + _timeDelay;
            }

            // check clock difference
            T dt = _targetTime - _runTime;
            if ( dt <= 0 ) { _targetTime += _timeDelay; }
            return dt;
        }

    private:
        T _timeDelay;
        T _runTime;
        T _targetTime;
    };

} // namespace coca

#endif // COCA_TIMER_H_INCLUDED
