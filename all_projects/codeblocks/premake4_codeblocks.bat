@echo off
cd "../../coca/build"
premake4 autogen
premake4 codeblocks
cd "../../cocaIrrlicht/build"
call premake4 codeblocks
cd "../../cocaRunner/build"
call premake4 codeblocks
cd "../../cocaUtils/build"
call premake4 codeblocks
cd "../../cocaXML/build"
call premake4 codeblocks
cd "../../cocaEditor/build"
call premake4 codeblocks
cd "../../cocaBullet/build"
call premake4 codeblocks
pause