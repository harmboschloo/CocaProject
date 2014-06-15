
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
		includedirs( { "../vendor/bullet/src" } )
		-- linker

	   -- This defines the bullet project
	   project( "bullet" )
			location( toolset )
			-- compiler
			files( { "../vendor/bullet/src/*.h", "../vendor/bullet/src/BulletCollision/**.*", "../vendor/bullet/src/BulletDynamics/**.*", "../vendor/bullet/src/LinearMath/**.*" } )
			-- linker
			kind( "StaticLib" )
			
			configuration( "debug" )
				-- compiler
				defines( { "_DEBUG" } )
				flags( { "Symbols" } )
				-- linker
				targetdir( coca.premake.getTargetDir() )
				
			configuration( "release" )
				-- compiler
				defines( { "NDEBUG" } )
				flags( { "OptimizeSpeed" } )
				-- linker
				targetdir( coca.premake.getTargetDir() )
				
	   -- This defines the project
	   project( COCA_PROJECT_NAME )
			location( toolset )
			-- compiler
			files( { "../source/**.h*", "../source/**.c*" } )
			includedirs( { coca.getIncludePath() } )
			-- linker
			kind( "SharedLib" )
			links( { "coca", "bullet" } )
			postbuildcommands( { coca.getCopyInterfaceCommand( "../../source/IBulletDebugDrawer.h" ) } )
			
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
			files( { "../test/**" } )
			includedirs( { coca.getIncludePath() } )
			-- linker
			kind( "ConsoleApp" )
			links( { "coca" } )

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
