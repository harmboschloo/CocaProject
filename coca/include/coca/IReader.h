#ifndef COCA_I_READER_H_INCLUDED
#define COCA_I_READER_H_INCLUDED

#include <istream>

namespace coca
{
    class ISystem;
    class INode;

    class IReader
    {
    public:
        virtual ~IReader() {}
        virtual coca::ISystem* readSystem( std::istream& stream ) = 0;
        virtual coca::INode* readNode( std::istream& stream ) = 0;
    };

} // namespace coca

#endif // COCA_I_READER_H_INCLUDED
