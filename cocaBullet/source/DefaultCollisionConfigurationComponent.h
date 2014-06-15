#ifndef DEFAULT_COLLISION_CONFIGURATION_COMPONENT_H_INCLUDED
#define DEFAULT_COLLISION_CONFIGURATION_COMPONENT_H_INCLUDED

#include <btBulletCollisionCommon.h>
#include <coca/IComponent.h>

class DefaultCollisionConfigurationComponent : public coca::IComponent
{
public:
    DefaultCollisionConfigurationComponent();
    ~DefaultCollisionConfigurationComponent();

    bool init( coca::INode& node );
    void onEnabled();
    void onDisabled();

private:
    btDefaultCollisionConfiguration* _configuration;
};

#endif // DEFAULT_COLLISION_CONFIGURATION_COMPONENT_H_INCLUDED
