#ifndef COCA_TO_STRING_COMPONENT_H_INCLUDED
#define COCA_TO_STRING_COMPONENT_H_INCLUDED

#include "../../IComponent.h"
#include "../../INode.h"
#include "../../attributes.h"
#include "../../utils/string.h"

namespace coca
{
    template<typename T>
    class ToStringComponent : public IComponent
    {
    public:
        ToStringComponent() :
                _dataStringOutput( 0 )
        {
        }

        ~ToStringComponent()
        {
        }

        bool init( coca::INode& node )
        {
            node.addAttribute( "data", coca::createInputAttribute<T>(
                                   coca::makeFunction( *this, &ToStringComponent<T>::update ) ) );

            _dataStringOutput =  coca::createOutputAttribute<std::string>( coca::makeAccessor( _dataString ) );
            node.addAttribute( "string", _dataStringOutput );

            return true;
        }

        void onEnabled()
        {
        }

        void onDisabled()
        {
        }

        void update( T data )
        {
            _dataString = toString( data );
            _dataStringOutput->update();
        }

    private:
        IAttribute* _dataStringOutput;
        std::string _dataString;
    };

} // namespace coca

#endif // COCA_TO_STRING_COMPONENT_H_INCLUDED
