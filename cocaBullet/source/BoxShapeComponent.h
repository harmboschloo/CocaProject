#ifndef BOX_SHAPE_COMPONENT_H_INCLUDED
#define BOX_SHAPE_COMPONENT_H_INCLUDED

#include "CollisionShapeComponent.h"

template<typename T = btBoxShape>
class BoxShapeComponent : public CollisionShapeComponent<T>
{
public:
    BoxShapeComponent()
    {
    }

    virtual ~BoxShapeComponent()
    {
    }

    virtual bool init( coca::INode& node )
    {
        if ( !CollisionShapeComponent<T>::init( node ) ) { return false; }

        coca::IAttribute* attribute = 0;

        attribute = coca::createInputOutputAttribute<btVector3>(
                        coca::makeAccessor( *static_cast<btBoxShape*>( &this->_shape ),
                                            &btBoxShape::getHalfExtents,
                                            &btBoxShape::setHalfExtents ) );
        node.addAttribute( "halfExtents", attribute );

        return true;
    }

    virtual void onEnabled()
    {
        CollisionShapeComponent<T>::onEnabled();
    }

    virtual void onDisabled()
    {
        CollisionShapeComponent<T>::onDisabled();
    }
};

#endif // BOX_SHAPE_COMPONENT_H_INCLUDED
