// Copyright (C) 2008-2009 Harm Boschloo

#include "NodeEvent.h"

namespace coca
{
    NodeEvent::NodeEvent( EEventType type, const INode& node ) :
            EventBase( type ),
            _node( &node ),
            _node2( 0 ),
            _attribute( 0 )
    {
    }

    NodeEvent::NodeEvent( EEventType type, const INode& node, const INode& node2 ) :
            EventBase( type ),
            _node( &node ),
            _node2( &node2 ),
            _attribute( 0 )
    {
    }

    NodeEvent::NodeEvent( EEventType type, const INode& node, const IAttribute& attribute ) :
            EventBase( type ),
            _node( &node ),
            _node2( 0 ),
            _attribute( &attribute )
    {
    }

    NodeEvent::~NodeEvent()
    {
    }

    const INode* NodeEvent::getNode() const
    {
        return _node;
    }

    const INode* NodeEvent::getNode2() const
    {
        return _node2;
    }

    const IAttribute* NodeEvent::getAttribute() const
    {
        return _attribute;
    }

} // namespace coca
