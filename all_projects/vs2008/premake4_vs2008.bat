@echo off
cd "../../coca/build"
premake4 autogen
premake4 vs2008
cd "../../cocaIrrlicht/build"
call premake4 vs2008
cd "../../cocaRunner/build"
call premake4 vs2008
cd "../../cocaUtils/build"
call premake4 vs2008
cd "../../cocaXML/build"
call premake4 vs2008
cd "../../cocaEditor/build"
call premake4 vs2008
cd "../../cocaBullet/build"
call premake4 vs2008
pause