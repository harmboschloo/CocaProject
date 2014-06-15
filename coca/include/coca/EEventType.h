// Copyright (C) 2007-2008 Harm Boschloo

#ifndef COCA_E_EVENT_TYPE_H_INCLUDED
#define COCA_E_EVENT_TYPE_H_INCLUDED

namespace coca
{
    enum EEventType
    {
        E_SYSTEM_DESCRIPTION_CHANGED_EVENT,     // 0
        E_SYSTEM_ROOT_CHANGED_EVENT,            // 1
        E_NODE_NAME_CHANGED_EVENT,              // 2
        E_NODE_DISABLED_COUNT_CHANGED_EVENT,    // 3
        E_NODE_COMPONENT_CHANGED_EVENT,         // 4
        E_NODE_CHILD_APPENDED_EVENT,            // 5
        E_NODE_CHILD_REMOVED_EVENT,             // 6
        E_NODE_CHILD_MOVED_UP_EVENT,            // 7
        E_NODE_CHILD_MOVED_DOWN_EVENT,          // 8
        E_NODE_ATTRIBUTE_ADDED_EVENT,           // 9
        E_NODE_ATTRIBUTE_REMOVED_EVENT,         // 10
        E_ATTRIBUTE_ENABLED_EVENT,              // 11
        E_ATTRIBUTE_DISABLED_EVENT,             // 12
        E_ATTRIBUTE_SINK_APPENDED_EVENT,        // 13
        E_ATTRIBUTE_SINK_REMOVED_EVENT,         // 14
        E_ATTRIBUTE_SINK_MOVED_UP_EVENT,        // 15
        E_ATTRIBUTE_SINK_MOVED_DOWN_EVENT,      // 16
        E_ATTRIBUTE_SOURCE_APPENDED_EVENT,      // 17
        E_ATTRIBUTE_SOURCE_REMOVED_EVENT,       // 18
        E_NUMBER_OF_EVENT_TYPES                 // 19
    };

} // namespace coca

#endif // COCA_E_EVENT_TYPE_H_INCLUDED
