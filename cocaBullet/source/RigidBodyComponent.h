#ifndef RIGID_BODY_COMPONENT_H_INCLUDED
#define RIGID_BODY_COMPONENT_H_INCLUDED

#include "CollisionObjectComponent.h"
#include "bullet_io.h"

template<typename T = btRigidBody>
class RigidBodyComponent : public CollisionObjectComponent<T>
{
public:
    RigidBodyComponent() :
            _rigidBody( 0 ),
            _rigidBodyAttribute( 0 )
    {
    }

    virtual ~RigidBodyComponent()
    {
    }

    virtual bool init( coca::INode& node )
    {
        if ( !CollisionObjectComponent<T>::init( node ) ) { return false; }

        coca::IAttribute* attribute = 0;

        attribute = coca::createInputOutputAttribute<btScalar>(
                        coca::makeAccessor( *this, &RigidBodyComponent::getMass,
                                            &RigidBodyComponent::setMass ) );
        node.addAttribute( "mass", attribute );

        attribute = coca::createInputOutputAttribute<btVector3>(
                        coca::makeAccessor( *this, &RigidBodyComponent::getInertia,
                                            &RigidBodyComponent::setInertia ) );
        node.addAttribute( "inertia", attribute );

        attribute = coca::createInputOutputAttribute<btVector3>(
                        coca::makeAccessor( this->_object, &btRigidBody::getLinearVelocity,
                                            &btRigidBody::setLinearVelocity ) );
        node.addAttribute( "linearVelocity", attribute );

        attribute = coca::createInputOutputAttribute<btVector3>(
                        coca::makeAccessor( this->_object, &btRigidBody::getAngularVelocity,
                                            &btRigidBody::setAngularVelocity ) );
        node.addAttribute( "angularVelocity", attribute );

        attribute = coca::createInputAttribute<btVector3>(
                        coca::makeFunction( this->_object, &btRigidBody::applyCentralImpulse ) );
        node.addAttribute( "applyCentralImpulse", attribute );

        attribute = coca::createInputAttribute<btVector3>(
                        coca::makeFunction( this->_object, &btRigidBody::applyCentralForce ) );
        node.addAttribute( "applyCentralForce", attribute );

        attribute = coca::createInputAttribute<btVector3>(
                        coca::makeFunction( this->_object, &btRigidBody::applyTorqueImpulse ) );
        node.addAttribute( "applyTorqueImpulse", attribute );

        attribute = coca::createInputAttribute<btVector3>(
                        coca::makeFunction( this->_object, &btRigidBody::applyTorque ) );
        node.addAttribute( "applyTorque", attribute );

        _rigidBodyAttribute = coca::createOutputAttribute<btRigidBody*>(
                                  coca::makeAccessor( _rigidBody ) );
        node.addAttribute( "rigidBody", _rigidBodyAttribute );

        // all ok
        return true;
    }

    virtual void onEnabled()
    {
        CollisionObjectComponent<T>::onEnabled();
        _rigidBody = &this->_object;
        _rigidBodyAttribute->update();
    }

    virtual void onDisabled()
    {
        _rigidBody = 0;
        _rigidBodyAttribute->update();
        CollisionObjectComponent<T>::onDisabled();
    }

    virtual void setCollisionShape( btCollisionShape* shape )
    {
        if ( shape != this->getCollisionShape() )
        {
            COCA_DEBUG_INFO( "setCollisionShape " << shape );
            this->_object.setCollisionShape( shape );
            this->_collisionObjectAttribute->update();
            _rigidBodyAttribute->update();
        }
    }

    btScalar getMass()
    {
        btScalar invMass = this->_object.getInvMass();
        return ( invMass == 0 ? invMass : 1/invMass );
    }

    void setMass( btScalar mass )
    {
        this->_object.setMassProps( mass, getInertia() );
    }

    btVector3 getInertia()
    {
        btVector3 vector = this->_object.getInvInertiaDiagLocal();
        if ( vector.x() != 0 ) { vector.setX( 1/vector.x() ); }
        if ( vector.y() != 0 ) { vector.setY( 1/vector.y() ); }
        if ( vector.z() != 0 ) { vector.setZ( 1/vector.z() ); }
        return vector;
    }

    void setInertia( const btVector3& inertia )
    {
        this->_object.setMassProps( getMass(), inertia );
    }

protected:
    btRigidBody* _rigidBody;
    coca::IAttribute* _rigidBodyAttribute;
};

#endif // RIGID_BODY_COMPONENT_H_INCLUDED
