#ifndef COLLISION_DISPATCHER_COMPONENT_H_INCLUDED
#define COLLISION_DISPATCHER_COMPONENT_H_INCLUDED

#include <btBulletCollisionCommon.h>
#include <coca/IComponent.h>
#include <coca/attributes/IAttribute.h>

class CollisionDispatcherComponent : public coca::IComponent
{
public:
    CollisionDispatcherComponent();
    ~CollisionDispatcherComponent();

    bool init( coca::INode& node );
    void onEnabled();
    void onDisabled();

    btCollisionConfiguration* getConfiguration() const;
    void setConfiguration( btCollisionConfiguration* configuration );

private:
    btCollisionConfiguration* _configuration;
    btCollisionDispatcher* _dispatcher;
    coca::IAttribute* _dispatcherAttribute;
};

#endif // COLLISION_DISPATCHER_COMPONENT_H_INCLUDED
