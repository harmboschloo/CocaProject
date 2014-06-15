#ifndef XML_READER_H_INCLUDED
#define XML_READER_H_INCLUDED

#include <coca/IReader.h>
#include <coca/attributes/IAttribute.h>
#include <coca/additional/xml.h>
#include <string>
#include <map>
#include <vector>
#include <utility>

class XMLReader : public coca::IReader
{
public:
    XMLReader();
    ~XMLReader();

    coca::ISystem* readSystem( std::istream& stream );
    coca::INode* readNode( std::istream& stream );

private:
    typedef std::map<const std::string,coca::IAttribute*> AttributeMap;
    typedef std::pair<coca::IAttribute*,std::string> ConnectionPair;
    typedef std::vector<ConnectionPair> ConnectionVector;

    bool readHeader();
    bool readFooter();
    coca::ISystem* readSystem();
    std::string readSystemDescription();
    coca::INode* readComponentNode();
    coca::INode* readCompositeNode();
    void readNodeAttributes( coca::INode& node );
    bool readAttribute( coca::INode& node );
    bool readInputValue( coca::IAttribute& attribute );
    bool readSink( coca::IAttribute& attribute );
    bool readWrapperAttribute( coca::INode& node );

    bool storeSink( std::string id, coca::IAttribute& sink );
    bool storeConnection( coca::IAttribute& source, const std::string& sinkId );
    void connectSinks();
    void connectSink( coca::IAttribute& source, const std::string& sinkId );
    void reset();

    coca::IrrXMLReader* _reader;
    AttributeMap _sinks;
    ConnectionVector _connections;
};

#endif // XML_READER_H_INCLUDED
