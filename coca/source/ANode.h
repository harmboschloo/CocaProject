// Copyright (C) 2008-2009 Harm Boschloo

#ifndef COCA_A_NODE_H_INCLUDED
#define COCA_A_NODE_H_INCLUDED

#include <coca/INode.h>

namespace coca
{
    class ANode : public INode
    {
    public:
        ANode( const std::string& name = "", INode* parent = 0 );
        virtual ~ANode();

        // Name //
        const std::string& getName() const;
        void setName( const std::string& name );

        // Parent and root //
        const INode* getParent() const;
        INode* getParent();
        bool isRoot() const;
        const INode& getRoot() const;
        INode& getRoot();

        // State //
        bool isEnabled() const;
        bool enable( bool enableChildren = true );
        bool isDisabled() const;
        void disable( bool disableChildren = true );
        int getDisabledCount() const;
        void setDisabledCount( int disabledCount );

        // Attributes //
        bool removeAttribute( IAttribute* attribute );
        bool removeAttribute( const std::string& name );
        void clearAttributes();
        bool hasAttribute( const IAttribute* attribute ) const;
        const std::string& findAttributeName( const IAttribute* attribute ) const;
        const IAttribute* findAttribute( const std::string& name ) const;
        IAttribute* findAttribute( const std::string& name );
        const AttributeMap& getAttributes() const;
        void setAttributesEnabled( bool enabled ) const;

        // Component //
        virtual bool isComponentNode() const;
        virtual bool resetComponent( const std::string& componentId = "" );
        virtual const std::string& getComponentId() const;
        virtual const IComponent* getComponent() const;
        virtual IComponent* getComponent();

        // Composite //
        virtual bool isCompositeNode() const;
        virtual const NodeVector& getChildren() const;
        virtual bool append( INode* child );
        virtual bool remove( INode* child );
        virtual void clearChildren();
        virtual bool moveUp( INode* child );
        virtual bool moveDown( INode* child );

    protected:
        static const NodeVector EMPTY_NODE_VECTOR;

        void setParent( INode* parent );
        void setOwnerOf( IAttribute* attribute );
        void destroy( IAttribute* attribute );
        //void removeAttributeFromParent( IAttribute* attribute );
        void setParentEnabled( IAttribute* attribute, bool enabled );

        virtual void onEnabled();
        virtual void onDisabled();
        virtual void doRemoveAttribute( AttributeMap::iterator it ) = 0;

        int _disabledCount;
        std::string _name;
        INode* _parent;
        AttributeMap _attributes;
    };

} // namespace coca

#endif // COCA_A_NODE_H_INCLUDED
