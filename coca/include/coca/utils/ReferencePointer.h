// Copyright (C) 2008-2009 Harm Boschloo

#ifndef COCA_REFERENCE_POINTER_H_INCLUDED
#define COCA_REFERENCE_POINTER_H_INCLUDED

#include "test.h"

namespace coca
{

    /** Wraps a pointer to a reference counted object, so we don't have to worry
        about grabbing and releasing.
    */
    template<typename T>
    class ReferencePointer
    {
    public:
        ReferencePointer( T* pointer = 0 ) : _pointer( pointer )
        {
            if ( _pointer ) { _pointer->grab(); }
        }

        ReferencePointer( T& reference ) : _pointer( &reference )
        {
            if ( _pointer ) { _pointer->grab(); }
        }

        ReferencePointer( const ReferencePointer<T>& refPointer ) : _pointer( refPointer.get() )
        {
            if ( _pointer ) { _pointer->grab(); }
        }

        template<typename X>
        ReferencePointer( const ReferencePointer<X>& refPointer ) : _pointer( refPointer.get() )
        {
            if ( _pointer ) { _pointer->grab(); }
        }

        ~ReferencePointer()
        {
            if ( _pointer ) { _pointer->drop(); }
        }

        ReferencePointer<T>& operator=( T* pointer )
        {
            reset( pointer );
            return *this;
        }

        ReferencePointer<T>& operator=( T& reference )
        {
            reset( &reference );
            return *this;
        }

        ReferencePointer<T>& operator=( const ReferencePointer<T>& refPointer )
        {
            reset( refPointer.get() );
            return *this;
        }

        template<typename X>
        ReferencePointer<T>& operator=( const ReferencePointer<X>& refPointer )
        {
            reset( refPointer.get() );
            return *this;
        }

        void reset( T* pointer = 0 )
        {
            if ( pointer ) { pointer->grab(); }
            if ( _pointer ) { _pointer->drop(); }
            _pointer = pointer;
        }

        T& operator*() const
        {
            COCA_ASSERT( _pointer != 0 );
            return *_pointer;
        }

        T* operator->() const
        {
            return _pointer;
        }

        T* get() const
        {
            return _pointer;
        }

        operator T*() const
        {
            return _pointer;
        }

    private:
        T* _pointer;
    };

} // namespace coca


#endif // COCA_REFERENCE_POINTER_H_INCLUDED
