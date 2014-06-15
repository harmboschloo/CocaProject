#include "SequentialImpulseConstraintSolverComponent.h"
#include <coca/INode.h>
#include <coca/attributes.h>

SequentialImpulseConstraintSolverComponent::SequentialImpulseConstraintSolverComponent() :
        _solver( 0 )
{
}

SequentialImpulseConstraintSolverComponent::~SequentialImpulseConstraintSolverComponent()
{
}

bool SequentialImpulseConstraintSolverComponent::init( coca::INode& node )
{
    coca::IAttribute* attribute = coca::createOutputAttribute<btConstraintSolver*>(
                                      coca::makeAccessor( _solver ) );
    node.addAttribute( "solver", attribute );

    return true;
}

void SequentialImpulseConstraintSolverComponent::onEnabled()
{
    COCA_ASSERT_EQUAL( _solver, ( void* )0 );
    _solver = new btSequentialImpulseConstraintSolver();
}

void SequentialImpulseConstraintSolverComponent::onDisabled()
{
    delete _solver;
    _solver = 0;
}
