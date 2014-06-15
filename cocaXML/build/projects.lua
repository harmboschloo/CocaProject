
function defineProjects( toolset )
	if not toolset then toolset = _ACTION end
	assert( toolset )
	assert( COCA_PROJECT_NAME )
	assert( COCA_LOCATION )

	-- indenting is purely cosmetic here
	-- A solution contains projects, and defines the available configurations
	solution( COCA_PROJECT_NAME )
		location( toolset )
		configurations(  { "debug", "release" } )
		-- compiler
		language( "C++" )
		flags( { "ExtraWarnings" } )
		includedirs( { coca.getIncludePath() } )
		-- linker
		links( { "coca" } )

	   -- This defines the project
	   project( COCA_PROJECT_NAME )
			location( toolset )
			-- compiler
			files( { "../source/**.h*", "../source/**.c*" } )
			-- linker
			kind( "SharedLib" )

			configuration( "debug" )
				-- compiler
				defines( { "_DEBUG" } )
				flags( { "Symbols" } )
				-- linker
				targetdir( coca.premake.getTargetDir() )
				libdirs( { coca.premake.getCocaLibDir() } )
				-- post build
				postbuildcommands( { coca.premake.getCopyPluginCommand() } )
				
			configuration( "release" )
				-- compiler
				defines( { "NDEBUG" } )
				flags( { "Optimize" } )
				-- linker
				targetdir( coca.premake.getTargetDir() )
				libdirs( { coca.premake.getCocaLibDir() } )
				-- post build
				postbuildcommands( { coca.premake.getCopyPluginCommand() } )
		
		-- This defines a test project
		project( COCA_PROJECT_NAME.."-test" )
			location( toolset )
			-- compiler
			files( { "../test/**", "../source/**.h*", "../source/**.c*" } )
			-- linker
			kind( "ConsoleApp" )
			
			configuration( "debug" )
				-- compiler
				defines( { "_DEBUG" } )
				flags( { "Symbols" } )
				-- linker
				targetdir( coca.premake.getTargetDir() )
				libdirs( { coca.premake.getCocaLibDir() } )
				
			configuration( "release" )
				-- compiler
				defines( { "NDEBUG" } )
				flags( { "Optimize" } )
				-- linker
				targetdir( coca.premake.getTargetDir() )
				libdirs( { coca.premake.getCocaLibDir() } )
end
