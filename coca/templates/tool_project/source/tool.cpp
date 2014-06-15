#include <coca/coca.h>

coca::StdLogMessageHandler _stdLog;
coca::FileLogMessageHandler _fileLog( "tool.log" );

int main( /*int argc, char* argv[]*/ )
{
    COCA_INFO( "Hello world!" );
    return 0;
}
