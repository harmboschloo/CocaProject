#ifndef DYNAMICS_WORLD_COMPONENT_H_INCLUDED
#define DYNAMICS_WORLD_COMPONENT_H_INCLUDED

#include "CollisionWorldComponent.h"
#include <btBulletDynamicsCommon.h>
#include <coca/utils/function/ConditionalAccessor.h>

class DynamicsWorldComponent : public CollisionWorldComponent
{
protected:
    typedef std::map< coca::AOutputAttribute<btRigidBody*>*, btRigidBody* > RigidBodyMap;

    DynamicsWorldComponent();
    virtual ~DynamicsWorldComponent();

    virtual bool init( coca::INode& node );
    virtual void destroyWorld();

    void init( btDynamicsWorld* world );
    void setAccessorObject( btDynamicsWorld* world );
    void simulateStep();

    bool onRigidBodyEvent( const coca::AttributeEvent<btRigidBody*>& event );
    void addRigidBodies();
    void addRigidBody( coca::AOutputAttribute<btRigidBody*>* body );
    void addRigidBody( btRigidBody* body );
    void removeRigidBody( coca::AOutputAttribute<btRigidBody*>* body );
    void removeRigidBody( btRigidBody* object );
    void updateRigidBody( coca::AOutputAttribute<btRigidBody*>* body );

    btScalar _timeStep;
    coca::IAttribute* _rigidBodyAttribute;
    btDynamicsWorld* _dynamicsWorld;
    coca::IAttribute* _dynamicsWorldAttribute;
    coca::IAttribute* _simulateStepAttribute;
    coca::ConditionalAccessor<btDynamicsWorld,btVector3> _gravityAccessor;
    RigidBodyMap _rigidBodies;
};

#endif // DYNAMICS_WORLD_COMPONENT_H_INCLUDED
