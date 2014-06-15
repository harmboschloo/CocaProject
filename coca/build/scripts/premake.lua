assert( coca )
assert( coca.premate == nil )
coca.premake = {}

function coca.premake.getToolset()
	assert( _ACTION )
	return _ACTION
end

function coca.premake.getConfiguration()
	local cnf = configuration()
	assert( cnf )
	assert( cnf.keywords )
	return cnf.keywords[1]
end

function coca.premake.getProjectKind()
	return configuration().kind or project().blocks[1].kind
end

function coca.premake.getProjectLocation()
	return project().location
end

function coca.premake.getTargetFilename()
	local kind = coca.premake.getProjectKind()
	local prefix  = ""
	local suffix  = ""

	if os.is( "windows" ) then
		if kind == "ConsoleApp" or kind == "WindowedApp" then
			suffix = ".exe"
		elseif kind == "SharedLib" then
			suffix = ".dll"
		elseif kind == "StaticLib" then
			suffix = ".lib"
		else
			assert( false )
		end
	elseif os.is( "linux" ) then
		if kind == "SharedLib" then
			prefix = "lib"
			suffix = ".so"
		elseif kind == "StaticLib" then
			prefix = "lib"
			suffix = ".a"
		else
			assert( false )
		end
	else
		assert( false )
	end
	
	return prefix..coca.getProjectName()..suffix
end

function coca.premake.getTargetDir()
	return coca.premake.getToolset().."/"..coca.premake.getConfiguration()
end

function coca.premake.getTargetFileLocation()
	return path.join( coca.premake.getTargetDir(), coca.premake.getTargetFilename() )
end

function coca.premake.getActualTargetFileLocation()
	return path.getrelative( coca.premake.getProjectLocation(), coca.premake.getTargetFileLocation() )
end

function coca.premake.getCocaLibDir()
	return path.join( coca.getLocation(), "build/"..coca.premake.getToolset().."/"..coca.premake.getConfiguration() )
end

function coca.premake.getCocaPluginDir()
	return path.join( coca.premake.getCocaLibDir(), "plugins" )
end

function coca.premake.getCopyTargetCommand( destinationDirectory )
	return coca.premake.getCopyCommand( coca.premake.getActualTargetFileLocation(), destinationDirectory )
end

function coca.premake.getCopyPluginCommand()
	return coca.premake.getCopyTargetCommand( path.join( coca.premake.getCocaLibDir(), "plugins" ) )
end

function coca.premake.getCopyToolCommand()
	return coca.premake.getCopyTargetCommand( coca.premake.getCocaLibDir() )
end

function coca.premake.getCopyCommand( sourceFile, destinationDirectory )
	local prefix = ""
	if string.sub( coca.premake.getToolset(), 1, 2 ) == "vs" then
		prefix = "call "
	end
	return prefix..coca.getCopyCommand( sourceFile, destinationDirectory )
end
