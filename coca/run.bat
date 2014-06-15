@echo off
setlocal

if not defined COCA_LOCATION goto :no_coca_location

echo 1. codeblocks - debug
echo 2. codeblocks - release
echo 3. vs2008 - debug
echo 4. vs2008 - release
set /p toolset=please specify toolset: 

if %toolset%==1 set location=codeblocks/debug
if %toolset%==2 set location=codeblocks/release
if %toolset%==3 set location=vs2008/debug
if %toolset%==4 set location=vs2008/release

if not defined location goto :no_toolset
set exe="%COCA_LOCATION%/build/%location%/cocaRun.exe"
if not exist %exe% goto :no_exe

call %exe% %1 %2 %3 %4 %5 %6 %7 %8 %9
goto :exit

:no_coca_location
echo No COCA_LOCATION found
goto :exit

:no_toolset
echo No toolset specified
goto :exit

:no_exe
echo Could not find %exe%
goto :exit

:exit
endlocal
pause