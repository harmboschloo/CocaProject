newaction( {
	trigger     = "autogen",
	description = "Automatically generates several header and test files",
} )

if premake.actions[_ACTION] then
	dofile( "scripts.lua" )
	dofile( "projects.lua" )
	
	local testBaseDir = "../test/autogen"
	local algorithmDir =  "../include/coca/utils/algorithm/autogen"
	local functionDir =  "../include/coca/utils/function/autogen"
	local maxLevel =  5
	local cocaNamespace = "coca"
	
	if _ACTION == "clean" then
		os.rmdir( algorithmDir )
		os.rmdir( functionDir )
		os.rmdir( testBaseDir )
		for action in pairs( premake.actions ) do os.rmdir( action ) end
	elseif _ACTION == "autogen" then
		coca.generateAlgorithmFiles( maxLevel, algorithmDir, cocaNamespace )
		coca.generateFunctionFiles( maxLevel, functionDir, cocaNamespace )
		coca.generateIncludeTestFiles( "../include/**.h", "../include", testBaseDir.."/include_test" )
	else
		defineProjects( _ACTION )
	end
else
	dofile( "scripts.lua" )
	dofile( "projects.lua" )
end
