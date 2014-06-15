// Copyright (C) 2008-2009 Harm Boschloo

#ifndef COCA_OBSERVABLE_H_INCLUDED
#define COCA_OBSERVABLE_H_INCLUDED

#include "NonCopyable.h"
#include "algorithm.h"
#include "function/autogen/function.h"
#include "algorithm/autogen/forAll.h"
#include <vector>

namespace coca
{
    template<typename Observer>
    class Observable : private NonCopyable
    {
    public:
        typedef std::vector<Observer*> ObserverVector;

        Observable() {};
        virtual ~Observable() {};

        void addObserver( Observer& observer )
        {
            _observers.push_back( &observer );
        }

        void removeObserver( Observer& observer )
        {
            eraseItem( _observers, &observer );
        }

        const ObserverVector& getObservers() const
        {
            return _observers;
        }

        template<typename MA1,typename A1>
        void notifyObservers( void ( Observer::*member ) ( MA1 ), A1& argument ) const
        {
            forAll( _observers, makePtrMemberFunction( member ), argument );
        }

        template<typename MA1, typename MA2, typename A1, typename A2>
        void notifyObservers( void ( Observer::*member ) ( MA1, MA2 ), A1& argument1, A2& argument2 ) const
        {
            forAll( _observers, makePtrMemberFunction( member ), argument1, argument2 );
        }

        template<typename MA1, typename MA2, typename MA3, typename A1, typename A2, typename A3>
        void notifyObservers( void ( Observer::*member ) ( MA1, MA2, MA3 ),
                              A1& argument1, A2& argument2, A3& argument3 ) const
        {
            forAll( _observers, makePtrMemberFunction( member ), argument1, argument2, argument3 );
        }

    private:
        ObserverVector _observers;
    };

} // namespace coca

#endif // COCA_OBSERVABLE_H_INCLUDED
