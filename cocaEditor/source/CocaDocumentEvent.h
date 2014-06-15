// Copyright (C) 2007-2008 Harm Boschloo

#ifndef I_COCA_DOCUMENT_EVENT_H_INCLUDED
#define I_COCA_DOCUMENT_EVENT_H_INCLUDED

#include <coca/INode.h>
#include <coca/IEvent.h>
#include <coca/attributes/IAttribute.h>
#include <coca/utils/ReferenceCounted.h>
#include <wx/object.h>

class CocaDocument;

class CocaDocumentEvent : public wxObject, public coca::ReferenceCounted
{
public:
    CocaDocumentEvent( int type );
    CocaDocumentEvent( int type, const coca::IAttribute& attribute );
    CocaDocumentEvent( const coca::IEvent& event );
    ~CocaDocumentEvent();

    int getType() const;

    const coca::INode* getNode() const;
    const coca::INode* getNode2() const;
    const coca::IAttribute* getAttribute() const;
    const coca::IAttribute* getAttribute2() const;

private:
    int _type;
    const coca::INode* _node;
    const coca::INode* _node2;
    const coca::IAttribute* _attribute;
    const coca::IAttribute* _attribute2;

private:
    DECLARE_CLASS( CocaDocumentEvent )
    DECLARE_NO_COPY_CLASS( CocaDocumentEvent )
};

#endif // I_COCA_DOCUMENT_EVENT_H_INCLUDED
