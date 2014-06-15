// Copyright (C) 2008-2009 Harm Boschloo

#include "EventManager.h"
#include "SystemEvent.h"
#include "NodeEvent.h"
#include "AttributeEvent.h"
#include <coca/utils/algorithm/eraseItem.h>

namespace coca
{
    EventManager::EventManager()
    {
    }

    EventManager::~EventManager()
    {
    }

    void EventManager::appendEventCallback( const EventCallback& callback )
    {
        //COCA_DEBUG_INFO( "appendEventCallback " << callback.getAdaptor() );
        _callbacks.push_back( callback );
    }

    void EventManager::removeEventCallback( const EventCallback& callback )
    {
        //COCA_DEBUG_INFO( "removeEventCallback " << callback.getAdaptor() );
        eraseItem( _callbacks, callback );
    }

    bool EventManager::sendEvent( EEventType type, const ISystem& system )
    {
        switch ( type )
        {
        case E_SYSTEM_DESCRIPTION_CHANGED_EVENT:
        case E_SYSTEM_ROOT_CHANGED_EVENT:
            {
                SystemEvent event( type, system );
                sendEvent( event );
                return true;
            }
        default:
            COCA_ERROR( "Invalid event type" );
            return false;
        }
    }

    bool EventManager::sendEvent( EEventType type, const INode& node )
    {
        switch ( type )
        {
        case E_NODE_NAME_CHANGED_EVENT:
        case E_NODE_DISABLED_COUNT_CHANGED_EVENT:
        case E_NODE_COMPONENT_CHANGED_EVENT:
            {
                NodeEvent event( type, node );
                sendEvent( event );
                return true;
            }
        default:
            COCA_ERROR( "Invalid event type" );
            return false;
        }
    }

    bool EventManager::sendEvent( EEventType type, const INode& node, const INode& node2 )
    {
        switch ( type )
        {
        case E_NODE_CHILD_APPENDED_EVENT:
        case E_NODE_CHILD_REMOVED_EVENT:
        case E_NODE_CHILD_MOVED_UP_EVENT:
        case E_NODE_CHILD_MOVED_DOWN_EVENT:
            {
                NodeEvent event( type, node, node2 );
                sendEvent( event );
                return true;
            }
        default:
            COCA_ERROR( "Invalid event type" );
            return false;
        }
    }

    bool EventManager::sendEvent( EEventType type, const INode& node, const IAttribute& attribute )
    {
        switch ( type )
        {
        case E_NODE_ATTRIBUTE_ADDED_EVENT:
        case E_NODE_ATTRIBUTE_REMOVED_EVENT:
            {
                NodeEvent event( type, node, attribute );
                sendEvent( event );
                return true;
            }
        default:
            COCA_ERROR( "Invalid event type" );
            return false;
        }
    }

    bool EventManager::sendEvent( EEventType type, const IAttribute& attribute )
    {
        switch ( type )
        {
        case E_ATTRIBUTE_ENABLED_EVENT:
        case E_ATTRIBUTE_DISABLED_EVENT:
            {
                AttributeEvent event( type, attribute );
                sendEvent( event );
                return true;
            }
        default:
            COCA_ERROR( "Invalid event type" );
            return false;
        }
    }

    bool EventManager::sendEvent( EEventType type, const IAttribute& attribute1, const IAttribute& attribute2 )
    {
        switch ( type )
        {
        case E_ATTRIBUTE_SINK_APPENDED_EVENT:
        case E_ATTRIBUTE_SINK_REMOVED_EVENT:
        case E_ATTRIBUTE_SINK_MOVED_UP_EVENT:
        case E_ATTRIBUTE_SINK_MOVED_DOWN_EVENT:
        case E_ATTRIBUTE_SOURCE_APPENDED_EVENT:
        case E_ATTRIBUTE_SOURCE_REMOVED_EVENT:
            {
                AttributeEvent event( type, attribute1, attribute2 );
                sendEvent( event );
                return true;
            }
        default:
            COCA_ERROR( "Invalid event type" );
            return false;
        }
    }

    void EventManager::sendEvent( const IEvent& event )
    {
        //COCA_DEBUG_INFO( "Sending event " << event.getType() << "; " << _callbacks.size() );

        CallbackVector::const_iterator it;
        for ( it = _callbacks.begin(); it != _callbacks.end(); ++it )
        {
            (*it)( event );
        }
    }

} // namespace coca
