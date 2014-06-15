// Copyright (C) 2007-2008 Harm Boschloo

#ifndef IRR_SCENE_MANAGER_COMPONENT_H_INCLUDED
#define IRR_SCENE_MANAGER_COMPONENT_H_INCLUDED

#include <irrlicht.h>
#include <coca/IComponent.h>
#include <coca/attributes/IAttribute.h>

class SceneManagerComponent : public coca::IComponent
{
public:
    SceneManagerComponent();
    ~SceneManagerComponent();

    bool init( coca::INode& node );
    void onEnabled();
    void onDisabled();

    void setDevice( irr::IrrlichtDevice* device );
    void drawAll();
    irr::scene::ISceneManager* getSceneManager() const;
    irr::scene::ISceneNode* getRootSceneNode() const;

private:
    irr::scene::ISceneManager* _manager;
    irr::scene::ISceneNode* _root;
    coca::IAttribute* _managerOutput;
    coca::IAttribute* _rootOutput;
};

#endif //IRR_SCENE_MANAGER_COMPONENT_H_INCLUDED
