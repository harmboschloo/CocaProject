#ifndef COLLISION_WORLD_COMPONENT_H_INCLUDED
#define COLLISION_WORLD_COMPONENT_H_INCLUDED

#include <btBulletCollisionCommon.h>
#include <coca/IComponent.h>
#include <coca/attributes/IAttribute.h>
#include <coca/attributes/AttributeEvent.h>
//#include <coca/utils/function/ConditionalAccessor.h>
#include <map>

class CollisionWorldComponent : public coca::IComponent
{
public:
    CollisionWorldComponent();
    virtual ~CollisionWorldComponent();

    virtual bool init( coca::INode& node );
    virtual void onEnabled();
    virtual void onDisabled();

    virtual void createWorld();
    virtual void destroyWorld();
    virtual void updateWorld();

protected:
    typedef std::map< coca::AOutputAttribute<btCollisionObject*>*, btCollisionObject* > CollisionObjectMap;

    void init( btCollisionWorld* world );
    void setAccessorObject( btCollisionWorld* world );
    void performDiscreteCollisionDetection();

    bool onCollisionObjectEvent( const coca::AttributeEvent<btCollisionObject*>& event );
    void addCollisionObjects();
    void addCollisionObject( coca::AOutputAttribute<btCollisionObject*>* source );
    void addCollisionObject( btCollisionObject* object );
    void removeCollisionObject( coca::AOutputAttribute<btCollisionObject*>* source );
    void removeCollisionObject( btCollisionObject* object );
    void updateCollisionObject( coca::AOutputAttribute<btCollisionObject*>* source );

    btDispatcher* _dispatcher;
    btCollisionConfiguration* _collisionConfiguration;
    btBroadphaseInterface* _broadphase;
    btCollisionWorld* _collisionWorld;
    coca::IAttribute* _collisionObjectAttribute;
    coca::IAttribute* _collisionWorldAttribute;
    coca::IAttribute* _performDiscreteCollisionDetectionAttribute;
    CollisionObjectMap _collisionObjects;
};

#endif // COLLISION_WORLD_COMPONENT_H_INCLUDED
