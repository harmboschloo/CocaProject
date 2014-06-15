// Copyright (C) 2008-2009 Harm Boschloo

#ifndef COCA_EVENT_MANAGER_H_INCLUDED
#define COCA_EVENT_MANAGER_H_INCLUDED

#include <coca/IEventManager.h>
#include <vector>

namespace coca
{
    class EventManager : public IEventManager
    {
    public:
        EventManager();
        ~EventManager();

        void appendEventCallback( const EventCallback& callback );
        void removeEventCallback( const EventCallback& callback );

        bool sendEvent( EEventType type, const ISystem& system );
        bool sendEvent( EEventType type, const INode& node );
        bool sendEvent( EEventType type, const INode& node, const INode& node2 );
        bool sendEvent( EEventType type, const INode& node, const IAttribute& attribute );
        bool sendEvent( EEventType type, const IAttribute& attribute );
        bool sendEvent( EEventType type, const IAttribute& attribute1, const IAttribute& attribute2 );

    private:
        typedef std::vector<EventCallback> CallbackVector;

        void sendEvent( const IEvent& event );

        CallbackVector _callbacks;
    };

} // namespace coca

#endif // COCA_EVENT_MANAGER_H_INCLUDED
