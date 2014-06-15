#ifndef SEQUENTIAL_IMPULSE_CONSTRAINT_SOLVER_COMPONENT_H_INCLUDED
#define SEQUENTIAL_IMPULSE_CONSTRAINT_SOLVER_COMPONENT_H_INCLUDED

#include <btBulletDynamicsCommon.h>
#include <coca/IComponent.h>

class SequentialImpulseConstraintSolverComponent : public coca::IComponent
{
public:
    SequentialImpulseConstraintSolverComponent();
    ~SequentialImpulseConstraintSolverComponent();

    bool init( coca::INode& node );
    void onEnabled();
    void onDisabled();

private:
    btSequentialImpulseConstraintSolver* _solver;
};

#endif // SEQUENTIAL_IMPULSE_CONSTRAINT_SOLVER_COMPONENT_H_INCLUDED
