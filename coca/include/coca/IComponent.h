// Copyright (C) 2008-2009 Harm Boschloo

#ifndef COCA_I_COMPONENT_H_INCLUDED
#define COCA_I_COMPONENT_H_INCLUDED

#include <string>

namespace coca
{
    class INode;

    class IComponent
    {
    public:
        virtual ~IComponent() {}

        virtual bool init( coca::INode& node ) = 0;
        virtual void onEnabled() = 0;
        virtual void onDisabled() = 0;
    };

} // namespace coca

#endif // COCA_I_COMPONENT_H_INCLUDED
