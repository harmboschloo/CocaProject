// Copyright (C) 2008-2009 Harm Boschloo

#include "System.h"
#include <coca/INode.h>
#include <coca/IEventManager.h>
#include <coca/interface.h>

namespace coca
{
        System::System( const std::string& description, INode* root ) :
            _root( root ),
            _description( description )
        {
        }

        System::~System()
        {
        }

        const std::string& System::getDescription() const
        {
            return _description;
        }

        void System::setDescription( const std::string& description )
        {
            if ( description == _description ) { return; }

            _description = description;
            getEventManager().sendEvent( E_SYSTEM_DESCRIPTION_CHANGED_EVENT, *this );
        }

        INode* System::getRoot() const
        {
            return _root;
        }

        void System::setRoot( INode* root )
        {
            if ( root == _root ) { return; }

            _root = root;
            getEventManager().sendEvent( E_SYSTEM_ROOT_CHANGED_EVENT, *this );
        }

} // namespace coca
