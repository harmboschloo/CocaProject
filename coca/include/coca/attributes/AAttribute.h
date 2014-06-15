// Copyright (C) 2008-2009 Harm Boschloo

#ifndef COCA_A_ATTRIBUTE_H_INCLUDED
#define COCA_A_ATTRIBUTE_H_INCLUDED

#include "IAttribute.h"
#include "EAttributeFlag.h"
#include "../IEventManager.h"
#include "../interface.h"
#include "../utils/Flags.h"
#include "../utils/type.h"
#include "../utils/algorithm.h"
#include "../utils/preprocessor.h"
#include "../utils/function/autogen/PtrMemberFunction1.h"

namespace coca
{
    class AAttribute : public IAttribute
    {
    public:
        AAttribute() :
                _flags( E_ATTRIBUTE_ENABLED_FLAG|E_ATTRIBUTE_PARENT_ENABLED_FLAG|E_ATTRIBUTE_AUTO_UPDATE_FLAG ),
                _owner( 0 ),
                _sources( 0 ),
                _sinks( 0 )
        {
        }

        // Properties //

        const INode* getOwner() const
        {
            return _owner;
        }

        INode* getOwner()
        {
            return _owner;
        }

        bool isInputAttribute() const
        {
            return ( _sources != 0 );
        }

        bool isMultipleInputAttribute() const
        {
            // when E_ATTRIBUTE_CAN_HAVE_MULTIPLE_SOURCES_FLAG is set it implies _sources != 0
            return ( _flags.isSet( E_ATTRIBUTE_CAN_HAVE_MULTIPLE_SOURCES_FLAG ) );
        }

        bool isOutputAttribute() const
        {
            return ( _sinks != 0 );
        }

        void setEnabled( bool enabled )
        {
            bool wasEnabled = isEnabled();

            _flags.set( E_ATTRIBUTE_ENABLED_FLAG, enabled );

            if ( wasEnabled == isEnabled() ) { return; }
            if ( wasEnabled ) { onDisabled(); }
            else { onEnabled(); }
        }

        bool isSetEnabled() const
        {
            return _flags.isSet( E_ATTRIBUTE_ENABLED_FLAG );
        }

        bool isEnabled() const
        {
            return _flags.areSet( E_ATTRIBUTE_ENABLED_FLAG|E_ATTRIBUTE_PARENT_ENABLED_FLAG );
        }

        bool isUpdating() const
        {
            return _flags.isSet( E_ATTRIBUTE_IS_UPDATING_FLAG );
        }

        void setAutoUpdate( bool autoUpdate )
        {
            _flags.set( E_ATTRIBUTE_AUTO_UPDATE_FLAG, autoUpdate );
        }

        bool hasAutoUpdate() const
        {
            return _flags.isSet( E_ATTRIBUTE_AUTO_UPDATE_FLAG );
        }

        // Value //
        const std::type_info& getTypeInfo() const
        {
            return onTypeRequest();
        }

        virtual bool canBeSetAsString() const
        {
            return isInputAttribute();
        }

        bool setAsString( const std::string& valueString )
        {
            if ( !onStringUpdate( valueString ) ) { return false; }
            if ( _flags.isSet( E_ATTRIBUTE_AUTO_UPDATE_FLAG ) ) { update(); }
            return true;
        }

        std::string getAsString() const
        {
            return onStringRequest();
        }

        // Sources //
        bool connectSource( IAttribute& output )
        {
            if ( &output == this || !isInputAttribute() || !output.isOutputAttribute()
                    || ( this->getTypeInfo() != output.getTypeInfo() ) ) { return false; }

            if ( !_flags.isSet( E_ATTRIBUTE_CAN_HAVE_MULTIPLE_SOURCES_FLAG ) ) { disconnectSources(); }

            output.appendSink( *this );
            appendSource( output );

            return true;
        }

        bool disconnectSource( IAttribute& output )
        {
            return ( removeSource( output ) | output.removeSink( *this ) );
        }

        void disconnectSources()
        {
            if ( !_sources || _sources->empty() ) { return; }

            AttributeVector::iterator it;
            for ( it = _sources->begin(); it != _sources->end(); )
            {
                IAttribute& output = **it;
                if ( isEnabled() && output.isEnabled() ) { this->onSourceDetach( output ); }
                output.removeSink( *this );
                it = eraseSource( it );
            }
        }
        const AttributeVector* getSources() const
        {
            return _sources;
        }

        // Sinks //
        bool connectSink( IAttribute& input )
        {
            return input.connectSource( *this );
        }

        bool disconnectSink( IAttribute& input )
        {
            return input.disconnectSource( *this );
        }

        void disconnectSinks()
        {
            if ( !_sinks || _sinks->empty() ) { return; }

            AttributeVector::iterator it;
            for ( it = _sinks->begin(); it != _sinks->end(); )
            {
                ( *it )->removeSource( *this );
                it = eraseSink( it );
            }
        }

        bool moveSinkUp( IAttribute& input )
        {
            if ( _sinks && moveUp( *_sinks, &input ) )
            {
                getEventManager().sendEvent( E_ATTRIBUTE_SINK_MOVED_UP_EVENT, *this, input );
                return true;
            }
            else { return false; }
        }

        bool moveSinkDown( IAttribute& input )
        {
            if ( _sinks && moveDown( *_sinks, &input ) )
            {
                getEventManager().sendEvent( E_ATTRIBUTE_SINK_MOVED_DOWN_EVENT, *this, input );
                return true;
            }
            else { return false; }
        }

        const AttributeVector* getSinks() const
        {
            return _sinks;
        }

        // Updates //
        bool update()
        {
            if ( !isEnabled() ) { return false; }

            if ( _flags.isSet( E_ATTRIBUTE_IS_UPDATING_FLAG ) )
            {
                COCA_WARNING( "Cyclic event! IAtribute::update() called while attribute was already updating." );
                return false;
            }

            _flags.set( E_ATTRIBUTE_IS_UPDATING_FLAG );

            callUpdateCallbacks();
            updateSinks();

            _flags.unset( E_ATTRIBUTE_IS_UPDATING_FLAG );

            return true;
        }

        void appendUpdateCallback( const Function<>& function )
        {
            _updateCallbacks.push_back( function );
        }

        void removeUpdateCallback( const Function<>& function )
        {
            eraseItem( _updateCallbacks, function );
        }

    protected:
        virtual ~AAttribute()
        {
            if ( _sources )
            {
                AttributeVector::iterator it;
                for ( it = _sources->begin(); it != _sources->end(); )
                {
                    ( *it )->removeSink( *this );
                    it = eraseSource( it );
                }
            }

            if ( _sinks )
            {
                AttributeVector::iterator it;
                for ( it = _sinks->begin(); it != _sinks->end(); )
                {
                    ( *it )->removeSource( *this );
                    it = eraseSink( it );
                }
            }

            delete _sources;
            delete _sinks;
        }

        void initSources( bool multipleSources )
        {
            if ( !_sources )
            {
                _sources = new AttributeVector();
                if ( multipleSources ) { _flags.set( E_ATTRIBUTE_CAN_HAVE_MULTIPLE_SOURCES_FLAG ); }
            }
        }

        void initSinks()
        {
            if ( !_sinks ) { _sinks = new AttributeVector(); }
        }

        void doSourceUpdate( IAttribute& output )
        {
            if ( isEnabled() && onSourceUpdate( output ) && hasAutoUpdate() ) { update(); }
        }

        virtual const std::type_info& onTypeRequest() const = 0;
        // Input attribute callbacks
        virtual void onSourceAttach( IAttribute& COCA_HIDE( output ) ) {}
        virtual void onSourceDetach( IAttribute& COCA_HIDE( output ) ) {}
        virtual bool onSourceUpdate( IAttribute& COCA_HIDE( output ) ) { return false; }
        virtual bool onStringUpdate( const std::string& COCA_HIDE( valueString ) ) { return false; }
        // Output attribute callbacks
        virtual std::string onStringRequest() const { return ""; }

    private:
        typedef std::vector<Function<> > UpdateCallbackVector;

        void setOwner( INode* owner )
        {
            _owner = owner;
        }

        void setParentEnabled( bool enabled )
        {
            bool wasEnabled = isEnabled();

            _flags.set( E_ATTRIBUTE_PARENT_ENABLED_FLAG, enabled );

            if ( wasEnabled == isEnabled() ) { return; }
            if ( wasEnabled ) { onDisabled(); }
            else { onEnabled(); }
        }

        void onEnabled()
        {
            COCA_ASSERT( isEnabled() );
            attachSinks();
            attachSources();
            getEventManager().sendEvent( E_ATTRIBUTE_ENABLED_EVENT, *this );
        }

        void onDisabled()
        {
            COCA_ASSERT( !isEnabled() );
            detachSinks();
            detachSources();
            getEventManager().sendEvent( E_ATTRIBUTE_DISABLED_EVENT, *this );
        }

        void attachSources()
        {
            if ( !_sources ) { return; }

            AttributeVector::iterator it;
            for ( it = _sources->begin(); it != _sources->end(); ++it )
            {
                if (( *it )->isEnabled() ) { onSourceAttach( **it ); }
            }
        }

        void attachSinks()
        {
            if ( !_sinks ) { return; }

            AttributeVector::iterator it;
            for ( it = _sinks->begin(); it != _sinks->end(); ++it )
            {
                if (( *it )->isEnabled() ) {( *it )->onSourceAttach( *this ); }
            }
        }

        void detachSources()
        {
            if ( !_sources ) { return; }

            AttributeVector::iterator it;
            for ( it = _sources->begin(); it != _sources->end(); ++it )
            {
                if (( *it )->isEnabled() ) { onSourceDetach( **it ); }
            }
        }

        void detachSinks()
        {
            if ( !_sinks ) { return; }

            AttributeVector::iterator it;
            for ( it = _sinks->begin(); it != _sinks->end(); ++it )
            {
                if (( *it )->isEnabled() ) {( *it )->onSourceDetach( *this ); }
            }
        }

        void updateSinks()
        {
            if ( !_sinks || !isEnabled() ) { return; }
            forAll( *_sinks, makePtrMemberFunction( &IAttribute::doSourceUpdate ), *this );
        }

        void callUpdateCallbacks() const
        {
            callAllRef( _updateCallbacks );
        }

        void appendSource( IAttribute& output )
        {
            if ( !_sources ) { return; }
            _sources->push_back( &output );
            if ( isEnabled() && output.isEnabled() ) { onSourceAttach( output ); }
            getEventManager().sendEvent( E_ATTRIBUTE_SOURCE_APPENDED_EVENT, *this, output );
        }

        bool removeSource( IAttribute& output )
        {
            if ( _sources )
            {
                AttributeVector::iterator it = find( *_sources, &output );
                if ( it != _sources->end() )
                {
                    if ( isEnabled() && output.isEnabled() ) { onSourceDetach( output ); }
                    eraseSource( it );
                    return true;
                }
            }
            return false;
        }

        AttributeVector::iterator eraseSource( AttributeVector::iterator it )
        {
            IAttribute* attribute = *it;
            AttributeVector::iterator it2 = _sources->erase( it );
            getEventManager().sendEvent( E_ATTRIBUTE_SOURCE_REMOVED_EVENT, *this, *attribute );
            return it2;
        }

        void appendSink( IAttribute& input )
        {
            if ( !_sinks ) { return; }
            _sinks->push_back( &input );
            getEventManager().sendEvent( E_ATTRIBUTE_SINK_APPENDED_EVENT, *this, input );
        }

        bool removeSink( IAttribute& input )
        {
            if ( _sinks )
            {
                if ( eraseItem( *_sinks, &input ) )
                {
                    getEventManager().sendEvent( E_ATTRIBUTE_SINK_REMOVED_EVENT, *this, input );
                    return true;
                }
            }
            return false;
        }

        AttributeVector::iterator eraseSink( AttributeVector::iterator it )
        {
            IAttribute* attribute = *it;
            AttributeVector::iterator it2 = _sinks->erase( it );
            getEventManager().sendEvent( E_ATTRIBUTE_SINK_REMOVED_EVENT, *this, *attribute );
            return it2;
        }

        Flags<uint8> _flags;
        INode* _owner;
        AttributeVector* _sources;
        AttributeVector* _sinks;
        UpdateCallbackVector _updateCallbacks;
    };

} // namespace coca

#endif //COCA_A_ATTRIBUTE_H_INCLUDED
