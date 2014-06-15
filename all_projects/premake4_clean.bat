@echo off
cd "../coca/build"
premake4 clean
cd "../../cocaBullet/build"
call premake4 clean
cd "../../cocaEditor/build"
call premake4 clean
cd "../../cocaIrrlicht/build"
call premake4 clean
cd "../../cocaRunner/build"
call premake4 clean
cd "../../cocaUtils/build"
call premake4 clean
cd "../../cocaXML/build"
call premake4 clean
pause