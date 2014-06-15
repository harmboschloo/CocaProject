// Copyright (C) 2008-2009 Harm Boschloo

#ifndef COCA_CALL_ALL_H_INCLUDED
#define COCA_CALL_ALL_H_INCLUDED

namespace coca
{
    template<typename Container>
    void callAllRef( const Container& container )
    {
        typename Container::const_iterator it;
        for ( it = container.begin(); it != container.end(); ++it )
        {
            ( *it )();
        }
    }

    template<typename Container>
    void callAllPtr( const Container& container )
    {
        typename Container::const_iterator it;
        for ( it = container.begin(); it != container.end(); ++it )
        {
            ( **it )();
        }
    }

} // namespace coca

#endif // COCA_CALL_ALL_H_INCLUDED

