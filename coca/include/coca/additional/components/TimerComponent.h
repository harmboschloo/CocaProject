#ifndef COCA_TIMER_COMPONENT_H_INCLUDED
#define COCA_TIMER_COMPONENT_H_INCLUDED

#include "../../IComponent.h"
#include "../../INode.h"
#include "../../attributes.h"
#include "../../IClock.h"
#include "../../utils/Timer.h"
#include "../../utils/function/autogen/Function0.h"

namespace coca
{
    template<typename T>
    class TimerComponent : public IComponent
    {
    public:
        TimerComponent() :
                _realTime( false ),
                _runTimeDifference( 0 ),
                _runTimeOutput( 0 ),
                _targetTimeOutput( 0 ),
                _runTimeDifferenceOutput( 0 ),
                _runOutput( 0 ),
                _callback( 0 )
        {
            _callback = coca::makeFunction( *this, &TimerComponent<T>::update );
        }

        ~TimerComponent()
        {
        }

        bool init( coca::INode& node )
        {
            node.addAttribute( "targetTimeDelay", coca::createInputOutputAttribute<T>(
                                   coca::makeAccessor( _timer, &coca::Timer<T>::getTargetTimeDelay,
                                                       &coca::Timer<T>::resetTargetTimeDelay ) ) );

            node.addAttribute( "realTime", coca::createInputOutputAttribute<bool>(
                                   coca::makeAccessor( _realTime ) ) );

            _runTimeOutput =  coca::createOutputAttribute<T>(
                                  coca::makeFunction( _timer, &coca::Timer<T>::getRunTime ) );
            COCA_ASSERT( _runTimeOutput );
            node.addAttribute( "runTime", _runTimeOutput );

            _targetTimeOutput =  coca::createOutputAttribute<T>(
                                     coca::makeFunction( _timer, &coca::Timer<T>::getTargetTime ) );
            COCA_ASSERT( _targetTimeOutput );
            node.addAttribute( "targetTime", _targetTimeOutput );

            _runTimeDifferenceOutput =  coca::createOutputAttribute<T>(
                                            coca::makeAccessor( _runTimeDifference ) );
            COCA_ASSERT( _runTimeDifferenceOutput );
            node.addAttribute( "runTimeDifference", _runTimeDifferenceOutput );

            _runOutput = coca::createOutputAttribute();
            COCA_ASSERT( _runOutput );
            node.addAttribute( "run", _runOutput );

            return true;
        }

        void onEnabled()
        {
            _timer.reset();
            coca::getClock().appendTickCallback( _callback );
        }

        void onDisabled()
        {
            coca::getClock().removeTickCallback( _callback );
        }

        void update()
        {
            runTimer();

            if ( _realTime )
            {
                while ( _runTimeDifference < 0 )
                {
                    T oldRunTimeDifference = _runTimeDifference;
                    runTimer();
                    if ( _runTimeDifference <= oldRunTimeDifference ) { break; } // else we'll get stuck
                }
            }
        }

    protected:
        void runTimer()
        {
            _runTimeDifference = _timer.run();
            _runTimeOutput->update();
            _targetTimeOutput->update();
            _runTimeDifferenceOutput->update();
            if ( _runTimeDifference <= 0 ) { _runOutput->update(); }
        }

        bool _realTime;
        T _runTimeDifference;
        IAttribute* _runTimeOutput;
        IAttribute* _targetTimeOutput;
        IAttribute* _runTimeDifferenceOutput;
        IAttribute* _runOutput;
        Timer<T> _timer;
        Function<> _callback;
    };

} // namespace coca

#endif // COCA_TIMER_COMPONENT_H_INCLUDED
