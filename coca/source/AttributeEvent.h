// Copyright (C) 2008-2009 Harm Boschloo

#ifndef COCA_ATTRIBUTE_EVENT_H_INCLUDED
#define COCA_ATTRIBUTE_EVENT_H_INCLUDED

#include "EventBase.h"

namespace coca
{
    class AttributeEvent : public EventBase
    {
    public:
        AttributeEvent( EEventType type, const IAttribute& attribute );
        AttributeEvent( EEventType type, const IAttribute& attribute1, const IAttribute& attribute2 );
        ~AttributeEvent();

        const IAttribute* getAttribute() const;
        const IAttribute* getAttribute2() const;

    private:
        const IAttribute* _attribute1;
        const IAttribute* _attribute2;
    };

} // namespace coca

#endif // COCA_ATTRIBUTE_EVENT_H_INCLUDED
