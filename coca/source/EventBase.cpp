// Copyright (C) 2008-2009 Harm Boschloo

#include "EventBase.h"

namespace coca
{

    EventBase::EventBase( EEventType type ) :
        _type( type )
    {
    }

    EventBase::~EventBase()
    {
    }

    EEventType EventBase::getType() const
    {
        return _type;
    }

    const ISystem* EventBase::getSystem() const
    {
        return 0;
    }

    const INode* EventBase::getNode() const
    {
        return 0;
    }

    const INode* EventBase::getNode2() const
    {
        return 0;
    }

    const IAttribute* EventBase::getAttribute() const
    {
        return 0;
    }

    const IAttribute* EventBase::getAttribute2() const
    {
        return 0;
    }

} // namespace coca
