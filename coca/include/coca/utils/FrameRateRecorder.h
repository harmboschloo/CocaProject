// Copyright (C) 2008-2009 Harm Boschloo

#ifndef COCA_FRAME_RATE_RECORDER_H_INCLUDED
#define COCA_FRAME_RATE_RECORDER_H_INCLUDED

#include <ctime>

namespace coca
{
    template<typename T>
    class FrameRateRecorder
    {
    public:
        FrameRateRecorder( T recordTime = 1 ) :
            _recordTime( 1 ),
            _frameRate( 0 ),
            _frameCount( 0 ),
            _startClock( 0 )
        {
            setRecordTime( recordTime );
        }

        ~FrameRateRecorder()
        {
        }

        T getRecordTime() const
        {
            return _recordTime;
        }

        void setRecordTime( T time )
        {
            if ( time <= 0 || time == _recordTime ) { return; }
            _recordTime = time;
            resetRecording();
        }

        T getFrameRate() const
        {
            return _frameRate;
        }

        void recordFrame()
        {
            if ( _startClock == 0 )
            {
                _frameRate = 0;
                _frameCount = 0;
                _startClock = clock();
            }
            else
            {
                clock_t tick = clock();
                T dt = ( tick - _startClock )/static_cast<T>( CLOCKS_PER_SEC );
                if ( dt >= _recordTime )
                {
                    _frameRate = _frameCount/dt;
                    _startClock = tick;
                    _frameCount = 0;
                }
            }
            ++_frameCount;
        }

        void resetRecording()
        {
            _startClock = 0;
            _frameRate = 0;
            _frameCount = 0;
        }

    private:
        T _recordTime;
        T _frameRate;
        size_t _frameCount;
        clock_t _startClock;
    };

} // namespace coca

#endif // COCA_FRAME_RATE_RECORDER_H_INCLUDED
