#ifndef EMPTY_SHAPE_COMPONENT_H_INCLUDED
#define EMPTY_SHAPE_COMPONENT_H_INCLUDED

#include "CollisionShapeComponent.h"

template<typename T = btEmptyShape>
class EmptyShapeComponent : public CollisionShapeComponent<T>
{
};

#endif // EMPTY_SHAPE_COMPONENT_H_INCLUDED
