// Copyright (C) 2008-2009 Harm Boschloo

#ifndef COCA_I_EVENT_MANAGER_H_INCLUDED
#define COCA_I_EVENT_MANAGER_H_INCLUDED

#include "EEventType.h"
#include "utils/function/autogen/Function1.h"

namespace coca
{
    class ISystem;
    class INode;
    class IAttribute;
    class IEvent;

    class IEventManager
    {
    public:
        typedef Function<void,const IEvent&> EventCallback;

        virtual ~IEventManager() {}

        virtual void appendEventCallback( const EventCallback& callback ) = 0;
        virtual void removeEventCallback( const EventCallback& callback ) = 0;

        virtual bool sendEvent( EEventType type, const ISystem& system ) = 0;
        virtual bool sendEvent( EEventType type, const INode& node ) = 0;
        virtual bool sendEvent( EEventType type, const INode& node, const INode& node2 ) = 0;
        virtual bool sendEvent( EEventType type, const INode& node, const IAttribute& attribute ) = 0;
        virtual bool sendEvent( EEventType type, const IAttribute& attribute ) = 0;
        virtual bool sendEvent( EEventType type, const IAttribute& attribute1, const IAttribute& attribute2 ) = 0;
    };

} // namespace coca

#endif // COCA_I_EVENT_MANAGER_H_INCLUDED
