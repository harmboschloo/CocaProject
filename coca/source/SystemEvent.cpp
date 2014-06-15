// Copyright (C) 2008-2009 Harm Boschloo

#include "SystemEvent.h"

namespace coca
{
    SystemEvent::SystemEvent( EEventType type, const ISystem& system ) :
            EventBase( type ),
            _system( &system )
    {
    }

    SystemEvent::~SystemEvent()
    {
    }

    const ISystem* SystemEvent::getSystem() const
    {
        return _system;
    }

} // namespace coca
