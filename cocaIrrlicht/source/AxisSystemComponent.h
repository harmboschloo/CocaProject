// Copyright (C) 2007-2008 Harm Boschloo

#ifndef IRR_AXIS_SYSTEM_COMPONENT_H_INCLUDED
#define IRR_AXIS_SYSTEM_COMPONENT_H_INCLUDED

#include "SceneNodeComponent.h"

class AxisSystemComponent : public SceneNodeComponent
{
public:
    AxisSystemComponent();
    ~AxisSystemComponent();

    bool createAndGrabNode( irr::scene::ISceneManager& manager );
};

#endif //IRR_AXIS_SYSTEM_COMPONENT_H_INCLUDED
