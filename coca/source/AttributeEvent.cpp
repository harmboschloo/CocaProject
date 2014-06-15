// Copyright (C) 2008-2009 Harm Boschloo

#include "AttributeEvent.h"

namespace coca
{
    AttributeEvent::AttributeEvent( EEventType type, const IAttribute& attribute ) :
            EventBase( type ),
            _attribute1( &attribute ),
            _attribute2( 0 )
    {
    }

    AttributeEvent::AttributeEvent( EEventType type, const IAttribute& attribute1, const IAttribute& attribute2 ) :
            EventBase( type ),
            _attribute1( &attribute1 ),
            _attribute2( &attribute2 )
    {
    }

    AttributeEvent::~AttributeEvent()
    {
    }

    const IAttribute* AttributeEvent::getAttribute() const
    {
        return _attribute1;
    }

    const IAttribute* AttributeEvent::getAttribute2() const
    {
        return _attribute2;
    }

} // namespace coca
