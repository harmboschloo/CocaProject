// Copyright (C) 2008-2009 Harm Boschloo

#ifndef COCA_COMPOSITE_NODE_H_INCLUDED
#define COCA_COMPOSITE_NODE_H_INCLUDED

#include "ANode.h"

namespace coca
{
    class CompositeNode : public ANode
    {
    public:
        CompositeNode( const std::string& name = "", INode* parent = 0 );
        ~CompositeNode();

        // Attributes //
        bool addAttribute( const std::string& name, IAttribute* attribute );

        // Composite //
        bool isCompositeNode() const;
        const NodeVector& getChildren() const;
        bool append( INode* child );
        bool remove( INode* child );
        void clearChildren();
        bool moveUp( INode* child );
        bool moveDown( INode* child );

    private:
        void doRemoveAttribute( AttributeMap::iterator it );

        NodeVector _children;
    };

} // namespace coca

#endif // COCA_COMPOSITE_NODE_H_INCLUDED
