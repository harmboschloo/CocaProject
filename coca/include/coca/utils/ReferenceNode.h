// Copyright (C) 2008-2009 Harm Boschloo

#ifndef COCA_REFERENCE_NODE_H_INCLUDED
#define COCA_REFERENCE_NODE_H_INCLUDED

#include "ReferenceCounted.h"
#include "ReferencePointer.h"
#include "staticCast.h"
#include "algorithm/eraseItem.h"
#include <vector>

namespace coca
{
    template<typename T>
    class ReferenceNode : public ReferenceCounted
    {
    public:
        typedef ReferencePointer<T> NodePointer;
        typedef std::vector<NodePointer> ChildVector;

        ReferenceNode() :
            _parent( 0 )
        {
        }

        virtual ~ReferenceNode()
        {
            clearChildren();
        }

        const T* getParent() const
        {
            return _parent;
        }

        T* getParent()
        {
            return _parent;
        }

        bool isRoot() const
        {
            return ( _parent == 0 );
        }

        const T& getRoot() const
        {
            if ( _parent ) { return _parent->getRoot(); }
            else { return me(); }
        }

        T& getRoot()
        {
            if ( _parent ) { return _parent->getRoot(); }
            else { return me(); }
        }

        const ChildVector& getChildren() const
        {
            return _children;
        }

        bool append( T* child )
        {
            NodePointer ptr( child ); // scoped reference count

            if ( !child || child == this || child->_parent == this ) { return false; }

            _children.push_back( ptr );
            if ( child->_parent ) { child->_parent->remove( child ); }
            child->_parent = &me();

            return true;
        }

        bool remove( T* child )
        {
            NodePointer ptr( child ); // scoped reference count

            if ( !child || child->_parent != this ) { return false; }
            if ( !eraseItem( _children, child ) ) { return false; }

            child->_parent = 0;

            return true;
        }

        void clearChildren()
        {
            typename ChildVector::iterator it;
            for ( it = _children.begin(); it != _children.end(); )
            {
                (*it)->_parent = 0;
                it = _children.erase( it );
            }
        }

    private:
        const T& me() const
        {
            return *staticCast<const T*>( this );
        }

        T& me()
        {
            return *staticCast<T*>( this );
        }

        T* _parent;
        ChildVector _children;
    };

} // namespace coca

#endif //COCA_REFERENCE_NODE_H_INCLUDED
