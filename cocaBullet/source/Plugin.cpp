#include "Plugin.h"
#include "DefaultCollisionConfigurationComponent.h"
#include "CollisionDispatcherComponent.h"
#include "DbvtBroadphaseComponent.h"
#include "SequentialImpulseConstraintSolverComponent.h"
#include "CollisionWorldComponent.h"
#include "DiscreteDynamicsWorldComponent.h"
#include "CollisionObjectComponent.h"
#include "RigidBodyComponent.h"
#include "EmptyShapeComponent.h"
#include "BoxShapeComponent.h"
#include "SphereShapeComponent.h"
#include "DebugDrawRouterComponent.h"
#include <coca/additional/components/ConstantComponent.h>

// This allows the framework to load the plugin dynamically, so it's important!
COCA_EXPORT_PLUGIN( Plugin );

Plugin::Plugin()
{
    _info.getVersion().set( 0, 0, 1, "alpha" );
    _info.setName( "cocaBulletPlugin" );
    _info.setAuthor( "H.W. Boschloo" );
    _info.setEmail( "harm@boschloo.net" );
    _info.setWebsite( "www.boschloo.net" );
    _info.setDescription( "An bullet plugin for the coca library." );
}

Plugin::~Plugin()
{
}

bool Plugin::init()
{
    coca::IFactory& factory = coca::getFactory();
    factory.registerComponent<coca::ConstantComponent<btScalar> >( "btScalar" );
    factory.registerComponent<coca::ConstantComponent<btVector3> >( "btVector3" );
    factory.registerComponent<coca::ConstantComponent<btQuaternion> >( "btQuaternion" );
    factory.registerComponent<DefaultCollisionConfigurationComponent>( "btDefaultCollisionConfiguration" );
    factory.registerComponent<CollisionDispatcherComponent>( "btCollisionDispatcher" );
    factory.registerComponent<DbvtBroadphaseComponent>( "btDbvtBroadphase" );
    factory.registerComponent<SequentialImpulseConstraintSolverComponent>( "btSequentialImpulseConstraintSolver" );
    factory.registerComponent<CollisionWorldComponent>( "btCollisionWorld" );
    factory.registerComponent<DiscreteDynamicsWorldComponent>( "btDiscreteDynamicsWorld" );
    factory.registerComponent<CollisionObjectComponent<> >( "btCollisionObject" );
    factory.registerComponent<RigidBodyComponent<> >( "btRigidBody" );
    factory.registerComponent<EmptyShapeComponent<> >( "btEmptyShape" );
    factory.registerComponent<BoxShapeComponent<> >( "btBoxShape" );
    factory.registerComponent<SphereShapeComponent<> >( "btSphereShape" );
    factory.registerComponent<DebugDrawRouterComponent>( "btDebugDrawRouter" );

    return true;
}

