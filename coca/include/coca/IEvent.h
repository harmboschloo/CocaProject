// Copyright (C) 2008-2009 Harm Boschloo

#ifndef COCA_I_EVENT_H_INCLUDED
#define COCA_I_EVENT_H_INCLUDED

#include "EEventType.h"

namespace coca
{
    class ISystem;
    class INode;
    class IAttribute;

    class IEvent
    {
    public:
        virtual ~IEvent() {}

        virtual EEventType getType() const = 0;
        virtual const ISystem* getSystem() const = 0;
        virtual const INode* getNode() const = 0;
        virtual const INode* getNode2() const = 0;
        virtual const IAttribute* getAttribute() const = 0;
        virtual const IAttribute* getAttribute2() const = 0;
    };

} // namespace coca

#endif // COCA_I_EVENT_H_INCLUDED
