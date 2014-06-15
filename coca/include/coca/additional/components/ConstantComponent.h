#ifndef COCA_CONSTANT_COMPONENT_H_INCLUDED
#define COCA_CONSTANT_COMPONENT_H_INCLUDED

#include "../../IComponent.h"
#include "../../INode.h"
#include "../../attributes.h"

namespace coca
{
    template<typename T>
    class ConstantComponent : public IComponent
    {
    public:
        ConstantComponent() :
                _data()
        {
        }

        ~ConstantComponent()
        {
        }

        bool init( coca::INode& node )
        {
            node.addAttribute( "data", coca::createInputOutputAttribute<T>( coca::makeAccessor( _data ) ) );
            return true;
        }

        void onEnabled()
        {
        }

        void onDisabled()
        {
        }

    private:
        T _data;
    };

} // namespace coca

#endif // COCA_CONSTANT_COMPONENT_H_INCLUDED
