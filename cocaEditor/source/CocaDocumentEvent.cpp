// Copyright (C) 2007-2008 Harm Boschloo

#include "CocaDocumentEvent.h"

IMPLEMENT_CLASS( CocaDocumentEvent, wxObject );

CocaDocumentEvent::CocaDocumentEvent( int type ):
    _type( type ),
    _node( 0 ),
    _node2( 0 ),
    _attribute( 0 ),
    _attribute2( 0 )
{
}

CocaDocumentEvent::CocaDocumentEvent( int type, const coca::IAttribute& attribute ):
    _type( type ),
    _node( 0 ),
    _node2( 0 ),
    _attribute( &attribute ),
    _attribute2( 0 )
{
}

CocaDocumentEvent::CocaDocumentEvent( const coca::IEvent& event ):
    _type( event.getType() ),
    _node( event.getNode() ),
    _node2( event.getNode2() ),
    _attribute( event.getAttribute() ),
    _attribute2( event.getAttribute2() )
{
}

CocaDocumentEvent::~CocaDocumentEvent()
{
}

int CocaDocumentEvent::getType() const
{
    return _type;
}

const coca::INode* CocaDocumentEvent::getNode() const
{
    return _node;
}

const coca::INode* CocaDocumentEvent::getNode2() const
{
    return _node2;
}

const coca::IAttribute* CocaDocumentEvent::getAttribute() const
{
    return _attribute;
}

const coca::IAttribute* CocaDocumentEvent::getAttribute2() const
{
    return _attribute2;
}
