// Copyright (C) 2008-2009 Harm Boschloo

#ifndef COCA_MOVE_H_INCLUDED
#define COCA_MOVE_H_INCLUDED

#include <algorithm>

namespace coca
{
    template<typename Container, typename Item>
    bool moveUp( Container& container, const Item& item )
    {
        if ( container.size() > 1 && container.front() != item )
        {
            typename Container::iterator it;
            for ( it = container.begin()+1; it != container.end(); ++it )
            {
                if ( *it == item )
                {
                    std::swap( *it, *( it-1 ) );
                    return true;
                }
            }
        }
        return false;
    }

    template<typename Container, typename Item>
    bool moveDown( Container& container, const Item& item )
    {
        if ( container.size() > 1 && container.back() != item )
        {
            typename Container::iterator it;
            typename Container::iterator end = container.end()-1;
            for ( it = container.begin(); it != end; ++it )
            {
                if ( *it == item )
                {
                    std::swap( *it, *( it+1 ) );
                    return true;
                }
            }
        }
        return false;
    }

} // namespace coca

#endif // COCA_MOVE_H_INCLUDED

