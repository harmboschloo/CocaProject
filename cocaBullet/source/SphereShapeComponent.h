#ifndef SPHERE_SHAPE_COMPONENT_H_INCLUDED
#define SPHERE_SHAPE_COMPONENT_H_INCLUDED

#include "CollisionShapeComponent.h"

template<typename T = btSphereShape>
class SphereShapeComponent : public CollisionShapeComponent<T>
{
public:
    SphereShapeComponent()
    {
    }

    virtual ~SphereShapeComponent()
    {
    }

    virtual bool init( coca::INode& node )
    {
        if ( !CollisionShapeComponent<T>::init( node ) ) { return false; }

        coca::IAttribute* attribute = 0;

        attribute = coca::createInputOutputAttribute<btScalar>(
                        coca::makeAccessor( *this,
                                            &SphereShapeComponent<T>::getRadius,
                                            &SphereShapeComponent<T>::setRadius ) );
        node.addAttribute( "radius", attribute );

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

    btScalar getRadius()
    {
        return this->_shape.getImplicitShapeDimensions().getX();
    }

    void setRadius( btScalar radius )
    {
        this->_shape.setUnscaledRadius( radius );
    }
};

#endif // SPHERE_SHAPE_COMPONENT_H_INCLUDED
