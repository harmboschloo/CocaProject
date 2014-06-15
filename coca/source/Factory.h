#ifndef COCA_FACTORY_H_INCLUDED
#define COCA_FACTORY_H_INCLUDED

#include <coca/IFactory.h>

namespace coca
{
    class Factory : public IFactory
    {
    public:
        Factory();
        ~Factory();

        // System //
        ISystem* createSystem( const std::string& description = "", INode* root = 0 ) const;

        // Nodes //
        INode* createComponentNode( const std::string& componentId = "",
                                    const std::string& name = "", INode* parent = 0 ) const;
        INode* createCompositeNode( const std::string& name = "", INode* parent = 0 ) const;

        // Components //
        bool registerComponent( const std::string& id, ComponentRegistry::Creator creator );
        void clearComponentRegistrations();
        IComponent* createComponent( const std::string& id ) const;
        const ComponentRegistry& getComponentRegistry() const;

        // Readers //
        bool registerReader( const std::string& extension, ReaderRegistry::Creator creator );
        void clearReaderRegistrations();
        IReader* createReader( const std::string& extension ) const;
        const ReaderRegistry& getReaderRegistry() const;

        // Writers //
        bool registerWriter( const std::string& extension, WriterRegistry::Creator creator );
        void clearWriterRegistrations();
        IWriter* createWriter( const std::string& extension ) const;
        const WriterRegistry& getWriterRegistry() const;

    private:
        ComponentRegistry _componentRegistry;
        ReaderRegistry _readerRegistry;
        WriterRegistry _writerRegistry;
    };

} // namespace coca

#endif // COCA_FACTORY_H_INCLUDED
