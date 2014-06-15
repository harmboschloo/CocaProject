// Copyright (C) 2008-2009 Harm Boschloo

#ifndef COCA_ERASE_ITEM_H_INCLUDED
#define COCA_ERASE_ITEM_H_INCLUDED

#include "find.h"

namespace coca
{
    template<typename Container, typename Item>
    bool eraseItem( Container& container, Item item )
    {
        typename Container::iterator it = find( container, item );
        if ( it == container.end() ) { return false; }
        container.erase( it );
        return true;
    }

} // namespace coca

#endif // COCA_ERASE_ITEM_H_INCLUDED

