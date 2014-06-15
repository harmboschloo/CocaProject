// Copyright (C) 2008-2009 Harm Boschloo

#ifndef COCA_FIND_H_INCLUDED
#define COCA_FIND_H_INCLUDED

#include <algorithm>

namespace coca
{
    template<typename Container, typename Item>
    typename Container::iterator find( Container& container, const Item& item )
    {
        return std::find( container.begin(), container.end(), item );
    }

    template<typename Container, typename Item>
    typename Container::const_iterator find( const Container& container, const Item& item )
    {
        return std::find( container.begin(), container.end(), item );
    }

    template<typename Container, typename Needle, typename Function>
    typename Container::iterator find( Container& container, const Needle& needle, Function function )
    {
        typename Container::iterator it;
        for ( it = container.begin(); it != container.end(); ++it )
        {
            if ( function( *it ) == needle ) { break; }
        }
        return it;
    }

    template<typename Container, typename Needle, typename Function>
    typename Container::const_iterator find( const Container& container, const Needle& needle, Function function )
    {
        typename Container::const_iterator it;
        for ( it = container.begin(); it != container.end(); ++it )
        {
            if ( function( *it ) == needle ) { break; }
        }
        return it;
    }

} // namespace coca

#endif // COCA_FIND_H_INCLUDED

