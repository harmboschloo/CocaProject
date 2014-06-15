// Copyright (C) 2007-2008 Harm Boschloo

#ifndef ATTRIBUTE_CONNECTION_MENU_H_INCLUDED
#define ATTRIBUTE_CONNECTION_MENU_H_INCLUDED

#include <coca/INode.h>
#include <wx/menu.h>

class AttributeConnectionMenu : public wxMenu
{
public:
    AttributeConnectionMenu( const coca::INode& sourceNode, const coca::IAttribute& sourceAttribute,
                             const coca::INode& sinkNode );
    AttributeConnectionMenu( const coca::INode& sourceNode,
                             const coca::INode& sinkNode, const coca::IAttribute& sinkAttribute );
    ~AttributeConnectionMenu();

    void onMenu( wxCommandEvent& event );

    bool hasConnections() const;
    const coca::IAttribute* getSelectedSource() const;
    const coca::IAttribute* getSelectedSink() const;

private:
    typedef std::pair<const coca::IAttribute*,const coca::IAttribute*> AttributePair;
    typedef std::vector<AttributePair> ConnectionVector;

    void reset();
    void reset( const coca::INode& sourceNode, const coca::IAttribute& sourceAttribute, const coca::INode& sinkNode );
    void reset( const coca::INode& sourceNode, const coca::INode& sinkNode, const coca::IAttribute& sinkAttribute );
    int getNextId() const;
    int toIndex( int id ) const;

    int _selectedConnection;
    ConnectionVector _connections;

    DECLARE_EVENT_TABLE()
};

#endif // ATTRIBUTE_CONNECTION_MENU_H_INCLUDED

