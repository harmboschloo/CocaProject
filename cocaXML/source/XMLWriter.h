#ifndef XML_WRITER_H_INCLUDED
#define XML_WRITER_H_INCLUDED

#include <coca/IWriter.h>
#include <coca/attributes/IAttribute.h>
#include <coca/additional/xml.h>
#include <vector>

class XMLWriter : public coca::IWriter
{
public:
    XMLWriter();
    ~XMLWriter();

    bool write( const coca::ISystem& system, std::ostream& stream );
    bool write( const coca::INode& node, std::ostream& stream );

private:
    typedef std::vector<const coca::IAttribute*> IdVector;

    void writeHeader();
    void writeFooter();
    void write( const coca::ISystem& system );
    void write( const coca::INode& node );
    void write( const coca::IAttribute& attribute, const std::string& name );
    void write( const coca::IAttribute& attribute, const std::string& name, const coca::INode& node );

    size_t findId( const coca::IAttribute& attribute );

    coca::XMLWriter* _writer;
    IdVector _ids;
};

#endif // XML_WRITER_H_INCLUDED
