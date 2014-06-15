// Copyright (C) 2008-2009 Harm Boschloo

#ifndef COCA_NODE_EVENT_H_INCLUDED
#define COCA_NODE_EVENT_H_INCLUDED

#include "EventBase.h"

namespace coca
{
    class NodeEvent : public EventBase
    {
    public:
        NodeEvent( EEventType type, const INode& node );
        NodeEvent( EEventType type, const INode& node, const INode& node2 );
        NodeEvent( EEventType type, const INode& node, const IAttribute& attribute );
        ~NodeEvent();

        const INode* getNode() const;
        const INode* getNode2() const;
        const IAttribute* getAttribute() const;

    private:
        const INode* _node;
        const INode* _node2;
        const IAttribute* _attribute;
    };

} // namespace coca

#endif // COCA_NODE_EVENT_H_INCLUDED
