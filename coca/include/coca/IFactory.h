#ifndef COCA_I_FACTORY_H_INCLUDED
#define COCA_I_FACTORY_H_INCLUDED

#include "interface.h"
#include "utils/create.h"
#include "utils/FactoryRegistry.h"
#include "utils/NonCopyable.h"
#include <string>
#include <map>

namespace coca
{
    class ISystem;
    class INode;
    class IComponent;
    class IReader;
    class IWriter;

    class IFactory : private NonCopyable
    {
    public:
        /** @name System */
        //@{
        virtual ISystem* createSystem( const std::string& description = "", INode* root = 0 ) const = 0;
        //@}

        /** @name Nodes */
        //@{
        virtual INode* createComponentNode( const std::string& componentId = "",
                                            const std::string& name = "", INode* parent = 0 ) const = 0;
        virtual INode* createCompositeNode( const std::string& name = "", INode* parent = 0 ) const = 0;
        //@}

        /** @name Components */
        //@{
        typedef FactoryRegistry<IComponent> ComponentRegistry;

        template<typename T>
        inline bool registerComponent( const std::string& id )
        {
            return registerComponent( id, &create<IComponent,T> );
        }

        virtual bool registerComponent( const std::string& id, ComponentRegistry::Creator creator ) = 0;
        virtual void clearComponentRegistrations() = 0;
        virtual IComponent* createComponent( const std::string& id ) const = 0;
        virtual const ComponentRegistry& getComponentRegistry() const = 0;
        //@}

        /** @name Readers */
        //@{
        typedef FactoryRegistry<IReader> ReaderRegistry;

        template<typename T>
        inline bool registerReader( const std::string& extension )
        {
            return registerReader( extension, &create<IReader,T> );
        }

        virtual bool registerReader( const std::string& extension, ReaderRegistry::Creator creator ) = 0;
        virtual void clearReaderRegistrations() = 0;

        virtual IReader* createReader( const std::string& extension ) const = 0;

        virtual const ReaderRegistry& getReaderRegistry() const = 0;
        //@}

        /** @name Writers */
        //@{
        typedef FactoryRegistry<IWriter> WriterRegistry;

        template<typename T>
        inline bool registerWriter( const std::string& extension )
        {
            return registerWriter( extension, &create<IWriter,T> );
        }

        virtual bool registerWriter( const std::string& extension, WriterRegistry::Creator creator ) = 0;
        virtual void clearWriterRegistrations() = 0;

        virtual IWriter* createWriter( const std::string& extension ) const = 0;

        virtual const WriterRegistry& getWriterRegistry() const = 0;
        //@}

    protected:
        virtual ~IFactory() {}
    };

} // namespace coca

#endif // COCA_I_FACTORY_H_INCLUDED
