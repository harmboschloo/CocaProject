// Copyright (C) 2008-2009 Harm Boschloo

#ifndef COCA_ATTRIBUTE_EVENT_H_INCLUDED
#define COCA_ATTRIBUTE_EVENT_H_INCLUDED

#include "EAttributeEventType.h"
#include <string>

namespace coca
{
    class IAttribute;
    template<typename ValueType> class AOutputAttribute;

    template<typename ValueType>
    class AttributeEvent
    {
    public:
        AttributeEvent( EAttributeEventType type, IAttribute& attribute,
                        AOutputAttribute<ValueType>& source ) :
                _type( type ),
                _attribute( &attribute ),
                _source( &source )
        {
        }

        AttributeEvent( EAttributeEventType type, IAttribute& attribute, const std::string& valueString ) :
                _type( type ),
                _attribute( &attribute ),
                _source( 0 ),
                _valueString( valueString )
        {
        }

        ~AttributeEvent()
        {
        }

        EAttributeEventType getType() const
        {
            return _type;
        }

        IAttribute& getAttribute() const
        {
            return _attribute;
        }

        AOutputAttribute<ValueType>* getSource() const
        {
            return _source;
        }

        const std::string& getValueString() const
        {
            return _valueString;
        }

    private:
        EAttributeEventType _type;
        IAttribute* _attribute;
        AOutputAttribute<ValueType>* _source;
        std::string _valueString;
    };

} // namespace coca

#endif //COCA_ATTRIBUTE_EVENT_H_INCLUDED
