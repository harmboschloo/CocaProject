// Copyright (C) 2008-2009 Harm Boschloo

#ifndef COCA_SYSTEM_H_INCLUDED
#define COCA_SYSTEM_H_INCLUDED

#include <coca/ISystem.h>
#include <coca/NodePtr.h>

namespace coca
{
    class System : public ISystem
    {
    public:
        System( const std::string& description = "", INode* root = 0 );
        ~System();

        const std::string& getDescription() const;
        void setDescription( const std::string& description );
        INode* getRoot() const;
        void setRoot( INode* root );

    private:
        NodePtr _root;
        std::string _description;
    };

} // namespace coca

#endif // COCA_SYSTEM_H_INCLUDED
