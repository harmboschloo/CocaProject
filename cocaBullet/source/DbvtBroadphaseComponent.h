#ifndef DBVT_BROADPHASE_COMPONENT_H_INCLUDED
#define DBVT_BROADPHASE_COMPONENT_H_INCLUDED

#include <btBulletCollisionCommon.h>
#include <coca/IComponent.h>
#include <coca/attributes/IAttribute.h>

class DbvtBroadphaseComponent : public coca::IComponent
{
public:
    DbvtBroadphaseComponent();
    ~DbvtBroadphaseComponent();

    bool init( coca::INode& node );
    void onEnabled();
    void onDisabled();

private:
    btDbvtBroadphase* _broadphase;
    coca::IAttribute* _broadphaseAttribute;
};

#endif // DBVT_BROADPHASE_COMPONENT_H_INCLUDED
