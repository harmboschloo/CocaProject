// Copyright (C) 2008-2009 Harm Boschloo

#ifndef COCA_SYSTEM_EVENT_H_INCLUDED
#define COCA_SYSTEM_EVENT_H_INCLUDED

#include "EventBase.h"

namespace coca
{
    class SystemEvent : public EventBase
    {
    public:
        SystemEvent( EEventType type, const ISystem& system );
        ~SystemEvent();

        const ISystem* getSystem() const;

    private:
        const ISystem* _system;
    };

} // namespace coca

#endif // COCA_SYSTEM_EVENT_H_INCLUDED
