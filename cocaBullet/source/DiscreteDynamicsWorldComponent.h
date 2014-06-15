#ifndef DISCRETE_DYNAMICS_WORLD_COMPONENT_H_INCLUDED
#define DISCRETE_DYNAMICS_WORLD_COMPONENT_H_INCLUDED

#include "DynamicsWorldComponent.h"

class DiscreteDynamicsWorldComponent : public DynamicsWorldComponent
{
public:
    DiscreteDynamicsWorldComponent();
    virtual ~DiscreteDynamicsWorldComponent();

    virtual bool init( coca::INode& node );

    virtual void createWorld();
    virtual void destroyWorld();

protected:
    void init( btDiscreteDynamicsWorld* world );

    btConstraintSolver* _constraintSolver;
    btDiscreteDynamicsWorld* _discreteDynamicsWorld;
    coca::IAttribute* _discreteDynamicsWorldAttribute;
};

#endif // DISCRETE_DYNAMICS_WORLD_COMPONENT_H_INCLUDED
