#ifndef COCA_I_WRITER_H_INCLUDED
#define COCA_I_WRITER_H_INCLUDED

#include <ostream>

namespace coca
{
    class ISystem;
    class INode;

    class IWriter
    {
    public:
        virtual ~IWriter() {}
        virtual bool write( const coca::ISystem& system, std::ostream& stream ) = 0;
        virtual bool write( const coca::INode& node, std::ostream& stream ) = 0;
    };

} // namespace coca

#endif // COCA_I_WRITER_H_INCLUDED
