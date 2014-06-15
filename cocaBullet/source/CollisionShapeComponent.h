#ifndef COLLISION_SHAPE_COMPONENT_H_INCLUDED
#define COLLISION_SHAPE_COMPONENT_H_INCLUDED

#include "bullet_io.h"
#include <btBulletCollisionCommon.h>
#include <coca/IComponent.h>
#include <coca/INode.h>
#include <coca/attributes.h>

template<typename T>
class CollisionShapeComponent : public coca::IComponent
{
protected:
    CollisionShapeComponent() :
            _collisionShape( 0 ),
            _collisionShapeAttribute( 0 )
    {
    }

    virtual ~CollisionShapeComponent()
    {
    }

    virtual bool init( coca::INode& node )
    {
        coca::IAttribute* attribute = 0;

        attribute = coca::createInputOutputAttribute<btVector3>(
                        coca::makeAccessor( *static_cast<btCollisionShape*>( &_shape ),
                                            &btCollisionShape::getLocalScaling,
                                            &btCollisionShape::setLocalScaling ) );
        node.addAttribute( "localScaling", attribute );

        attribute = coca::createInputOutputAttribute<btScalar>(
                        coca::makeAccessor( *static_cast<btCollisionShape*>( &_shape ),
                                            &btCollisionShape::getMargin,
                                            &btCollisionShape::setMargin ) );
        node.addAttribute( "margin", attribute );

        _collisionShapeAttribute = coca::createOutputAttribute<btCollisionShape*>(
                                        coca::makeAccessor( _collisionShape ) );
        node.addAttribute( "collisionShape", _collisionShapeAttribute );

        // all ok
        return true;
    }

    virtual void onEnabled()
    {
        _collisionShape = &_shape;
        _collisionShapeAttribute->update();
    }

    virtual void onDisabled()
    {
        _collisionShape = 0;
        _collisionShapeAttribute->update();
    }

protected:
    T _shape;
    btCollisionShape* _collisionShape;
    coca::IAttribute* _collisionShapeAttribute;
};

#endif // COLLISION_SHAPE_COMPONENT_H_INCLUDED
