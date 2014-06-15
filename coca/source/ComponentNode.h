// Copyright (C) 2008-2009 Harm Boschloo

#ifndef COCA_COMPONENT_NODE_H_INCLUDED
#define COCA_COMPONENT_NODE_H_INCLUDED

#include "ANode.h"

namespace coca
{
    class ComponentNode : public ANode
    {
    public:
        ComponentNode( const std::string& componentId, const std::string& name = "", INode* parent = 0 );
        ~ComponentNode();

        // Attributes //
        bool addAttribute( const std::string& name, IAttribute* attribute );

        // Component //
        bool isComponentNode() const;
        bool resetComponent( const std::string& componentId = "" );
        const std::string& getComponentId() const;
        const IComponent* getComponent() const;
        IComponent* getComponent();

    private:
        void onEnabled();
        void onDisabled();
        void doRemoveAttribute( AttributeMap::iterator it );
        void setParentEnabledForAttributes( bool enabled );

        IComponent* _component;
        std::string _componentId;
    };

} // namespace coca

#endif // COCA_COMPONENT_NODE_H_INCLUDED
