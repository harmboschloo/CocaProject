#include <windows.h>
//#include <iostream>

int main( /*int argc, char* argv[]*/ )
{
    DWORD dwReturnValue = 0;
    SendMessageTimeout( HWND_BROADCAST, WM_SETTINGCHANGE, 0,
                        ( LPARAM ) "Environment", SMTO_ABORTIFHUNG,
                        2000, &dwReturnValue );

    /*if ( dwReturnValue == 0 )
    {
        std::cerr << "Setting changed broadcast failed." << std::endl;
        return 1;
    }
    else
    {
        std::cout << "Setting changed broadcast successful." << std::endl;
        return 0;
    }*/
}
