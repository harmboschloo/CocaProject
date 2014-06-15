// Copyright (C) 2008-2009 Harm Boschloo

#ifndef COCA_EVENT_BASE_H_INCLUDED
#define COCA_EVENT_BASE_H_INCLUDED

#include <coca/IEvent.h>

namespace coca
{
    class EventBase : public IEvent
    {
    public:
        EventBase( EEventType type );
        virtual ~EventBase();

        EEventType getType() const;
        virtual const ISystem* getSystem() const;
        virtual const INode* getNode() const;
        virtual const INode* getNode2() const;
        virtual const IAttribute* getAttribute() const;
        virtual const IAttribute* getAttribute2() const;

    protected:
        EEventType _type;
    };

} // namespace coca

#endif // COCA_EVENT_BASE_H_INCLUDED
