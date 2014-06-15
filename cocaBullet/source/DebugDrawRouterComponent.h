#ifndef DEBUG_DRAW_ROUTER_COMPONENT_H_INCLUDED
#define DEBUG_DRAW_ROUTER_COMPONENT_H_INCLUDED

#include <coca/IComponent.h>
#include <coca/attributes/IAttribute.h>

class DebugDrawRouter;

class DebugDrawRouterComponent : public coca::IComponent
{
public:
    DebugDrawRouterComponent();
    ~DebugDrawRouterComponent();

    bool init( coca::INode& node );
    void onEnabled();
    void onDisabled();

private:
    DebugDrawRouter* _router;
};

#endif // DEBUG_DRAW_ROUTER_COMPONENT_H_INCLUDED

