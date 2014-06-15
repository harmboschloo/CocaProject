// Copyright (C) 2008-2009 Harm Boschloo

#ifndef COCA_I_SYSTEM_H_INCLUDED
#define COCA_I_SYSTEM_H_INCLUDED

#include <string>

namespace coca
{
    class INode;

    class ISystem
    {
    public:
        virtual ~ISystem() {}

        virtual const std::string& getDescription() const = 0;
        virtual void setDescription( const std::string& description ) = 0;
        virtual INode* getRoot() const = 0;
        virtual void setRoot( INode* root ) = 0;
    };

} // namespace coca

#endif // COCA_I_SYSTEM_H_INCLUDED
