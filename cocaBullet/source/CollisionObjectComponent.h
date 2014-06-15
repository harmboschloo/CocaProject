#ifndef COLLISION_OBJECT_COMPONENT_H_INCLUDED
#define COLLISION_OBJECT_COMPONENT_H_INCLUDED

#include "bullet_io.h"
#include <btBulletCollisionCommon.h>
#include <coca/IComponent.h>
#include <coca/INode.h>
#include <coca/attributes.h>
#include <coca/log/log.h>
#include <coca/utils/math.h>

template<typename T = btCollisionObject>
class CollisionObjectComponent : public coca::IComponent
{
public:
    CollisionObjectComponent() :
            _collisionObject( 0 ),
            _collisionObjectAttribute( 0 )
    {
    }

    virtual ~CollisionObjectComponent()
    {
    }

    virtual bool init( coca::INode& node )
    {
        coca::IAttribute* attribute = 0;

        attribute = coca::createInputOutputAttribute<btVector3>(
                        coca::makeAccessor( *this, &CollisionObjectComponent::getOrigin,
                                            &CollisionObjectComponent::setOrigin ) );
        node.addAttribute( "origin", attribute );

        attribute = coca::createInputOutputAttribute<btQuaternion>(
                        coca::makeAccessor( *this, &CollisionObjectComponent::getRotation,
                                            &CollisionObjectComponent::setRotation ) );
        node.addAttribute( "rotation", attribute );

        attribute = coca::createInputOutputAttribute<btVector3>(
                        coca::makeAccessor( *this, &CollisionObjectComponent::getEulerYPR,
                                            &CollisionObjectComponent::setEulerYPR ) );
        node.addAttribute( "rotationEulerYPR", attribute );

        attribute = coca::createInputOutputAttribute<btVector3>(
                        coca::makeAccessor( *this, &CollisionObjectComponent::getEulerZYX,
                                            &CollisionObjectComponent::setEulerZYX ) );
        node.addAttribute( "rotationEulerZYX", attribute );

        attribute = coca::createInputOutputAttribute<btCollisionShape*>(
                        coca::makeAccessor( *this, &CollisionObjectComponent::getCollisionShape,
                                            &CollisionObjectComponent::setCollisionShape ) );
        node.addAttribute( "collisionShape", attribute );

        attribute = coca::createInputOutputAttribute<btScalar>(
                        coca::makeAccessor( *static_cast<btCollisionObject*>( &_object ),
                                            &btCollisionObject::getRestitution,
                                            &btCollisionObject::setRestitution ) );
        node.addAttribute( "restitution", attribute );

        attribute = coca::createInputOutputAttribute<btScalar>(
                        coca::makeAccessor( *static_cast<btCollisionObject*>( &_object ),
                                            &btCollisionObject::getFriction,
                                            &btCollisionObject::setFriction ) );
        node.addAttribute( "friction", attribute );

        _collisionObjectAttribute = coca::createOutputAttribute<btCollisionObject*>(
                                        coca::makeAccessor( _collisionObject ) );
        node.addAttribute( "collisionObject", _collisionObjectAttribute );

        // all ok
        return true;
    }

    virtual void onEnabled()
    {
        _collisionObject = &_object;
        _collisionObjectAttribute->update();
    }

    virtual void onDisabled()
    {
        _collisionObject = 0;
        _collisionObjectAttribute->update();
    }

    const btVector3& getOrigin()
    {
        return _object.getWorldTransform().getOrigin();
    }

    void setOrigin( const btVector3& origin )
    {
        _object.getWorldTransform().setOrigin( origin );
    }

    btQuaternion getRotation()
    {
        return _object.getWorldTransform().getRotation();
    }

    void setRotation( const btQuaternion& rotation )
    {
        COCA_DEBUG_INFO( "setRotation: " << rotation );
        _object.getWorldTransform().setRotation( rotation );
        COCA_DEBUG_INFO( "getRotation: " << getRotation() );
    }

    btVector3 getEulerYPR()
    {
        btVector3 rotation;
        _object.getWorldTransform().getBasis().getEulerYPR( rotation.m_floats[0], rotation.m_floats[1], rotation.m_floats[2] );
         return ( rotation*coca::RAD2DEG );
    }

    void setEulerYPR( const btVector3& rotation )
    {
        COCA_DEBUG_INFO( "setEulerYPR: " << rotation );
        _object.getWorldTransform().getBasis().setEulerYPR(
            rotation.getX()*coca::DEG2RAD,
            rotation.getY()*coca::DEG2RAD,
            rotation.getZ()*coca::DEG2RAD );
        COCA_DEBUG_INFO( "getEulerYPR: " << getEulerYPR() );
    }

    btVector3 getEulerZYX()
    {
        btVector3 rotation;
        _object.getWorldTransform().getBasis().getEulerZYX( rotation.m_floats[2], rotation.m_floats[1], rotation.m_floats[0] );
        return ( rotation*coca::RAD2DEG );
    }

    void setEulerZYX( const btVector3& rotation )
    {
        COCA_DEBUG_INFO( "setEulerZYX: " << rotation );
        _object.getWorldTransform().getBasis().setEulerZYX(
            rotation.getX()*coca::DEG2RAD,
            rotation.getY()*coca::DEG2RAD,
            rotation.getZ()*coca::DEG2RAD );
        COCA_DEBUG_INFO( "getEulerZYX: " << getEulerZYX() );
    }

    btCollisionShape* getCollisionShape()
    {
        return _object.getCollisionShape();
    }

    virtual void setCollisionShape( btCollisionShape* shape )
    {
        if ( shape != getCollisionShape() )
        {
            COCA_DEBUG_INFO( "setCollisionShape " << shape );
            _object.setCollisionShape( shape );
            _collisionObjectAttribute->update();
        }
    }

protected:
    T _object;
    btCollisionObject* _collisionObject;
    coca::IAttribute* _collisionObjectAttribute;
};

#endif // COLLISION_OBJECT_COMPONENT_H_INCLUDED
