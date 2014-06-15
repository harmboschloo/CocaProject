// Copyright (C) 2008-2009 Harm Boschloo

#ifndef COCA_I_NODE_H_INCLUDED
#define COCA_I_NODE_H_INCLUDED

#include "NodePtr.h"
#include "utils/ReferenceCounted.h"
#include <string>
#include <vector>
#include <map>

namespace coca
{
    class IComponent;
    class IAttribute;
    class CompositeNode;

    /** ... */
    class INode : public ReferenceCounted
    {
    public:
        virtual ~INode() {}

        /** @name Name */
        //@{
        /** Returns the node's name. */
        virtual const std::string& getName() const = 0;
        /** Sets the node name. */
        virtual void setName( const std::string& name ) = 0;
        //@}

        /** @name Parent and root */
        //@{
        /** Returns the node's parent. */
        virtual const INode* getParent() const = 0;
        /** Returns the node's parent. */
        virtual INode* getParent() = 0;
        /** Returns whether the node is . */
        virtual bool isRoot() const = 0;
         /** Returns the root of this node. */
        virtual const INode& getRoot() const = 0;
         /** Returns the root of this node. */
        virtual INode& getRoot() = 0;
        //@}

        /** @name State */
        //@{
        /** Returns whether the node is enabled. */
        virtual bool isEnabled() const = 0;

        /** Decreases the disabled count by 1 when the count is greater than 0.
            When the count goes to 0 onEnabled is called.
            Returns whether the node is enabled, see enabled.
            Note, the disabled count can not be less than that of the node's parent. */
        virtual bool enable( bool enableChildren = true ) = 0;

        /** return whether the node is disabled. */
        virtual bool isDisabled() const = 0;

        /** Increases the disabled count with 1.
            When the count was 0 onDisabled() is called.
            Note, the disabled count of a child node can not be less than that of it's parent.
            If it is disable( false ) is automatically called on the child node. */
        virtual void disable( bool disableChildren = true ) = 0;

        /** Returns the disabled count. */
        virtual int getDisabledCount() const = 0;

        /** Sets the disabled count. No event is generated and no check are done. */
        virtual void setDisabledCount( int disabledCount ) = 0;
        //@}

        /** @name Attributes */
        //@{
        typedef std::map<const std::string,IAttribute*> AttributeMap;
        /** Addes an attribute to the node.
            When the node is a component node, it takes ownership of the pointer.
            Otherwise it will not.*/
        virtual bool addAttribute( const std::string& name, IAttribute* attribute ) = 0;
        virtual bool removeAttribute( IAttribute* attribute ) = 0;
        virtual bool removeAttribute( const std::string& name ) = 0;
        virtual void clearAttributes() = 0;
        virtual bool hasAttribute( const IAttribute* attribute ) const = 0;
        virtual const std::string& findAttributeName( const IAttribute* attribute ) const = 0;
        virtual const IAttribute* findAttribute( const std::string& name ) const = 0;
        virtual IAttribute* findAttribute( const std::string& name ) = 0;
        virtual const AttributeMap& getAttributes() const = 0;
        virtual void setAttributesEnabled( bool enabled ) const = 0;
        //@}

        /** @name Component
            These functions are specific for a component node. */
        //@{
        /** Returns whether this node is a component node.
            A component node can have one component, but no child nodes. */
        virtual bool isComponentNode() const = 0;
        /** Node takes ownership of the component! */
        virtual bool resetComponent( const std::string& componentId = "" ) = 0;
        /** Returns the unique id of the component. */
        virtual const std::string& getComponentId() const = 0;
        virtual const IComponent* getComponent() const = 0;
        virtual IComponent* getComponent() = 0;
        //@}

        /** @name Composite
            These functions are specific for a composite node. */
        //@{
        typedef std::vector<NodePtr> NodeVector;
        /** Returns whether this node is a composite node.
            A composite can have child nodes but no component. */
        virtual bool isCompositeNode() const = 0;
        /** Returns the child nodes of this component. */
        virtual const NodeVector& getChildren() const = 0;
        /** Appends a child node. */
        virtual bool append( INode* child ) = 0;
        /** Removes a child node. */
        virtual bool remove( INode* child ) = 0;
        /** Removes all child nodes. */
        virtual void clearChildren() = 0;
        /** Moves child up. */
        virtual bool moveUp( INode* child ) = 0;
        /** Moves child down. */
        virtual bool moveDown( INode* child ) = 0;
        //@}

    protected:
        friend class CompositeNode;

        virtual void setParent( INode* parent ) = 0;
    };

} // namespace coca

#endif // COCA_I_NODE_H_INCLUDED
