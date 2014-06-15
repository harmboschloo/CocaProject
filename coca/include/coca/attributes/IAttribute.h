// Copyright (C) 2008-2009 Harm Boschloo

#ifndef COCA_I_ATTRIBUTE_H_INCLUDED
#define COCA_I_ATTRIBUTE_H_INCLUDED

#include "../utils/NonCopyable.h"
#include "../utils/function/autogen/Function0.h"
#include <string>
#include <vector>
#include <typeinfo>

namespace coca
{
    class INode;

    class IAttribute : private NonCopyable
    {
    public:
        typedef std::vector<IAttribute*> AttributeVector;

        /** @name Properties */
        //@{
        /// The owner.
        virtual const INode* getOwner() const = 0;

        /// The owner.
        virtual INode* getOwner() = 0;

        /// Can have Sources.
        virtual bool isInputAttribute() const = 0;

        /// Can have multiple Sources.
        virtual bool isMultipleInputAttribute() const = 0;

        /// Can have Sinks.
        virtual bool isOutputAttribute() const = 0;

        /// Enable attribute.
        virtual void setEnabled( bool enabled ) = 0;

        /// Is attribute set enabled. See enable() and disable().
        virtual bool isSetEnabled() const = 0;

        /** Is attribute enabled.
            @return True if the attribute is set enable and the attribute's parent is enabled,
            false otherwise. */
        virtual bool isEnabled() const = 0;

        /// Whether the attribute is currently updating, see update().
        virtual bool isUpdating() const = 0;

        /**  Sets whether update() should be automatically called when the attributes value
         get updated (by setAsString() or a source update). */
        virtual void setAutoUpdate( bool autoUpdate ) = 0;

        /**  Whether update() is automatically called when the attributes value
         get updated (by setAsString() or a source update). */
        virtual bool hasAutoUpdate() const = 0;
        //@}

        /** @name Value */
        //@{
        virtual const std::type_info& getTypeInfo() const = 0;
        virtual bool canBeSetAsString() const = 0;
        virtual bool setAsString( const std::string& valueString ) = 0;
        virtual std::string getAsString() const = 0;
        //@}

        /** @name Sources */
        //@{
        virtual bool connectSource( IAttribute& output ) = 0;
        virtual bool disconnectSource( IAttribute& output ) = 0;
        virtual void disconnectSources() = 0;
        virtual const AttributeVector* getSources() const = 0;
        //@}

        /** @name Sinks */
        //@{
        virtual bool connectSink( IAttribute& input ) = 0;
        virtual bool disconnectSink( IAttribute& input ) = 0;
        virtual void disconnectSinks() = 0;
        virtual bool moveSinkUp( IAttribute& input ) = 0;
        virtual bool moveSinkDown( IAttribute& input ) = 0;
        virtual const AttributeVector* getSinks() const = 0;
        //@}

        /** @name Updates */
        //@{
        virtual bool update() = 0;
        virtual void appendUpdateCallback( const Function<>& function ) = 0;
        virtual void removeUpdateCallback( const Function<>& function ) = 0;
        //@}

    protected:
        friend class AAttribute;
        friend class ANode;

        virtual ~IAttribute() {}

        virtual void initSources( bool multipleSources ) = 0;
        virtual void initSinks() = 0;
        virtual void doSourceUpdate( IAttribute& output ) = 0;

        virtual const std::type_info& onTypeRequest() const = 0;
        // Input attribute callbacks
        virtual void onSourceAttach( IAttribute& output ) = 0;
        virtual void onSourceDetach( IAttribute& output ) = 0;
        virtual bool onSourceUpdate( IAttribute& output ) = 0;
        virtual bool onStringUpdate( const std::string& valueString ) = 0;
        // Output attribute callbacks
        virtual std::string onStringRequest() const = 0;

    private:
        virtual void setOwner( INode* owner ) = 0;
        virtual void setParentEnabled( bool enabled ) = 0;
        virtual void appendSource( IAttribute& output ) = 0;
        virtual bool removeSource( IAttribute& output ) = 0;
        virtual void appendSink( IAttribute& input ) = 0;
        virtual bool removeSink( IAttribute& input ) = 0;
    };

} // namespace coca

#endif //COCA_I_ATTRIBUTE_H_INCLUDED
