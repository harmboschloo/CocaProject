#define WIN32_LEAN_AND_MEAN

#include <coca/IFactory.h>
#include <coca/IClock.h>
#include <coca/ISystem.h>
#include <coca/IReader.h>
#include <coca/IWriter.h>
#include <coca/plugin/IPluginManager.h>
#include <coca/log.h>
#include <windows.h>
#include <memory>
#include <ctime>
#include <cstdlib>

coca::StdLogMessageHandler _stdLog;
coca::FileLogMessageHandler _fileLog( "coca.log" );
std::string _filename;
bool _writeSystem = false;
bool _displayTickRate = false;
std::auto_ptr<coca::ISystem> _system;
HWND _consoleWindow = 0;

enum Argument
{
    E_EXECUTABLE_ARGUMENT,
    E_FILENAME_ARGUMENT,
    E_OPTION_ARGUMENTS
};

enum ExitCode
{
    E_EXIT_SUCCESS = EXIT_SUCCESS,          // 0
    E_EXIT_INVALID_NUMBER_OF_ARGUMENTS,     // 1
    E_EXIT_UNABLE_TO_OPEN_FILE_FOR_READING, // 2
    E_EXIT_UNABLE_TO_OPEN_FILE_FOR_WRITING, // 3
    E_EXIT_NO_READER_FOR_EXTENSION,         // 4
    E_EXIT_NO_WRITER_FOR_EXTENSION,         // 5
    E_EXIT_SYSTEM_READ_FAILURE,             // 6
    E_EXIT_SYSTEM_WRITE_FAILURE,            // 7
    E_EXIT_SYSTEM_START_FAILURE,            // 8
    E_EXIT_STD_EXCEPTION,                   // 9
    E_EXIT_UNKNOWN_EXCEPTION,               // 10

};

void printInfo()
{
    COCA_INFO( "\n"
             << "coca filename [-w] [-c]\n"
             << "  filename   The filename containing the coca system you want to run.\n"
             << "  -w         Writes the coca system to the file again after running.\n"
             << "  -c         Logs clock tick rate changes.\n" );
}

void processArguments( int argc, char* argv[] )
{
    if ( argc < 2 )
    {
        COCA_ERROR( "Invalid number of arguments." );
        printInfo();
        exit( E_EXIT_INVALID_NUMBER_OF_ARGUMENTS );
    }

    _filename = argv[E_FILENAME_ARGUMENT];

    if ( argc <= E_OPTION_ARGUMENTS ) { return; }

    for ( int i = E_OPTION_ARGUMENTS; i < argc; ++i )
    {
        if ( strcmp( argv[i], "-w" ) == 0 )
        {
            _writeSystem = true;
        }
        if ( strcmp( argv[i], "-c" ) == 0 )
        {
            _displayTickRate = true;
        }
    }
}

void readSystemFromFile()
{
    std::ifstream fileStream( _filename.c_str() );

    if ( !fileStream.is_open() )
    {
        COCA_ERROR( "Could not open file for reading: " << _filename );
        exit( E_EXIT_UNABLE_TO_OPEN_FILE_FOR_READING );
    }

    // load plugins
    char exePath[MAX_PATH];
    ::GetModuleFileName( NULL,exePath,MAX_PATH );
    std::string pluginDirectory = coca::extractDirectory( exePath ) + "/plugins";

    int count = coca::getPluginManager().loadPlugins( pluginDirectory );
    COCA_INFO( count << " plugins loaded" );

    // find reader
    std::string extension = coca::extractExtension( _filename );
    std::auto_ptr<coca::IReader> reader( coca::getFactory().createReader( extension ) );
    if ( !reader.get() )
    {
        COCA_ERROR( "No reader found for extension: " << extension );
        exit( E_EXIT_NO_READER_FOR_EXTENSION );
    }

    COCA_INFO( "Reading file: " << _filename );
    _system.reset( reader->readSystem( fileStream ) );
    if ( !_system.get() )
    {
        COCA_ERROR( "Could not read coca system from file : " << _filename );
        exit( E_EXIT_SYSTEM_READ_FAILURE );
    }
}

void runSystem()
{
    COCA_ASSERT( _system.get() );

    if ( !_system->getRoot() )
    {
        COCA_INFO( "The component system is empty!" );
        return;
    }

    COCA_INFO( "Running... Press Esc to stop." );

    coca::IClock& clock = coca::getClock();
    float lastTickRate = clock.getTickRate();
    while ( !clock.stopped() )
    {
        clock.tick();

        // tick rate info
        if ( _displayTickRate )
        {
            float tickRate = clock.getTickRate();
            if ( tickRate != lastTickRate )
            {
                COCA_INFO( "Clock tick rate: " << tickRate );
                lastTickRate = tickRate;
            }
        }

        // Break when Esc is pressed with this console window in the foreground.
        // Otherwise if we have two console windows running, both exit.
        if ( GetKeyState( VK_ESCAPE ) < 0 && _consoleWindow == GetForegroundWindow() ) { break; }

        //if ( clock.getTickClock() == ::clock() ) { Sleep( 1 ); }
        //else { Sleep( 0 ); }
        Sleep( 0 );
    }

    COCA_INFO( "Stopped." );
}

void writeSystem()
{
    COCA_ASSERT( _system.get() );

    std::ofstream fileStream( _filename.c_str() );

    if ( !fileStream.is_open() )
    {
        COCA_ERROR( "Could not open file for writing: " << _filename );
        exit( E_EXIT_UNABLE_TO_OPEN_FILE_FOR_WRITING );
    }

    // find writer
    std::string extension = coca::extractExtension( _filename );
    std::auto_ptr<coca::IWriter> writer( coca::getFactory().createWriter( extension ) );
    if ( !writer.get() )
    {
        COCA_ERROR( "No writer found for extension: " << extension );
        exit( E_EXIT_NO_WRITER_FOR_EXTENSION );
    }

    COCA_INFO( "Writing file: " << _filename );
    if ( !writer->write( *_system, fileStream ) )
    {
        COCA_ERROR( "Could not write coca system to file : " << _filename );
        exit( E_EXIT_SYSTEM_WRITE_FAILURE );
    }
    COCA_INFO( "Writing finished successfully." );
}

void doPostRunOptions()
{
    if ( _writeSystem ) { writeSystem(); }
}

int main( int argc, char* argv[] )
{
    // assuming console window is on the foreground on startup
    _consoleWindow = GetForegroundWindow();

    try
    {
        processArguments( argc, argv );
        readSystemFromFile();
        runSystem();
        doPostRunOptions();
        return E_EXIT_SUCCESS;
    }
    catch( const std::exception& exception )
    {
        COCA_ERROR( "Exception: " << exception.what() );
        return E_EXIT_STD_EXCEPTION;
    }
    catch( ... )
    {
        COCA_ERROR( "Unknown exception." );
        return E_EXIT_UNKNOWN_EXCEPTION;
    }
}
