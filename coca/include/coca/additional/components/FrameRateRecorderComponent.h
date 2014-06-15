#ifndef COCA_FRAME_RATE_RECORDER_COMPONENT_H_INCLUDED
#define COCA_FRAME_RATE_RECORDER_COMPONENT_H_INCLUDED

#include "../../IComponent.h"
#include "../../INode.h"
#include "../../attributes.h"
#include "../../utils/FrameRateRecorder.h"

namespace coca
{
    template<typename T>
    class FrameRateRecorderComponent : public IComponent
    {
    public:
        FrameRateRecorderComponent() :
                _lastFrameRate( 0 ),
                _frameRateOutput( 0 )
        {
        }

        ~FrameRateRecorderComponent()
        {
        }

        bool init( coca::INode& node )
        {
            node.addAttribute( "recordFrame", coca::createInputAttribute<void>(
                                   coca::makeFunction( *this, &FrameRateRecorderComponent<T>::recordFrame ) ) );

            node.addAttribute( "recordTime", coca::createInputOutputAttribute<T>(
                                   coca::makeAccessor( _recorder, &coca::FrameRateRecorder<T>::getRecordTime,
                                                       &coca::FrameRateRecorder<T>::setRecordTime ) ) );

            _frameRateOutput =  coca::createOutputAttribute<T>( coca::makeAccessor( _lastFrameRate ) );
            COCA_ASSERT( _frameRateOutput );
            node.addAttribute( "frameRate", _frameRateOutput );

            return true;
        }

        void onEnabled()
        {
            _recorder.resetRecording();
        }

        void onDisabled()
        {
        }

        void recordFrame()
        {
            _recorder.recordFrame();
            if ( _recorder.getFrameRate() != _lastFrameRate )
            {
                _lastFrameRate = _recorder.getFrameRate();
                _frameRateOutput->update();
            }
        }

    private:
        T _lastFrameRate;
        IAttribute* _frameRateOutput;
        FrameRateRecorder<T> _recorder;
    };

} // namespace coca

#endif // COCA_FRAME_RATE_RECORDER_COMPONENT_H_INCLUDED
