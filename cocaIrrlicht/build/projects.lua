
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
		defines( { "_IRR_STATIC_LIB_" } )
		if os.is( "windows" ) then 
			defines( { "WIN32", "_WINDOWS" } ) 
		end
		includedirs( { coca.getIncludePath(), "../vendor/irrlicht/include" } )
		-- linker

	   -- This defines the irrlicht 3rdparth project
	   project( "irrlicht_3rdparty" )
			location( toolset )
			-- compiler
			language( "C" )
			files( { "../vendor/irrlicht/source/Irrlicht/libpng/*.h*", "../vendor/Irrlicht/source/Irrlicht/libpng/*.c*" } )
			files( { "../vendor/irrlicht/source/Irrlicht/zlib/*.h*", "../vendor/Irrlicht/source/Irrlicht/zlib/*.c*" } )
			files( { "../vendor/irrlicht/source/Irrlicht/jpeglib/*.h*", "../vendor/irrlicht/source/Irrlicht/jpeglib/*.c*" } )
			excludes( { "../vendor/irrlicht/source/Irrlicht/**example*" } )
			excludes( { "../vendor/irrlicht/source/Irrlicht/jpeglib/*cdjpeg*" } )
			excludes( { "../vendor/irrlicht/source/Irrlicht/jpeglib/*ansi*" } )
			excludes( { "../vendor/irrlicht/source/Irrlicht/jpeglib/*dos.c" } )
			excludes( { "../vendor/irrlicht/source/Irrlicht/jpeglib/*mac.c" } )
			includedirs( { "../vendor/irrlicht/source/Irrlicht/zlib" } )
			-- linker
			kind( "StaticLib" )

			configuration( "debug" )
				-- compiler
				defines( { "_DEBUG" } )
				flags( { "Symbols" } )
				buildoptions( { "-O0" } )
				-- linker
				targetdir( coca.premake.getTargetDir() )
				
			configuration( "release" )
				-- compiler
				defines( { "NDEBUG" } )
				flags( { "OptimizeSpeed" } )
				buildoptions( { "-ffast-math" } )
				-- linker
				targetdir( coca.premake.getTargetDir() )
				
	   -- This defines the irrlicht project
	   project( "irrlicht" )
			location( toolset )
			-- compiler
			language( "C++" )
			files( { "../vendor/irrlicht/include/*.h*", "../vendor/irrlicht/source/Irrlicht/*.h*", "../vendor/Irrlicht/source/Irrlicht/*.c*" } )
			if ( os.is( "windows" ) and toolset == "codeblocks" ) then 
				defines( { "__GNUWIN32__" } )
			end
			includedirs( { "../vendor/irrlicht/source/Irrlicht/zlib" } )
			-- linker
			kind( "StaticLib" )
			
			configuration( "debug" )
				-- compiler
				defines( { "_DEBUG" } )
				flags( { "Symbols" } )
				buildoptions( { "-O0" } )
				-- linker
				targetdir( coca.premake.getTargetDir() )
				
			configuration( "release" )
				-- compiler
				defines( { "NDEBUG" } )
				flags( { "OptimizeSpeed" } )
				buildoptions( { "-ffast-math" } )
				-- linker
				targetdir( coca.premake.getTargetDir() )
				
	   -- This defines the project
	   project( COCA_PROJECT_NAME )
			location( toolset )
			-- compiler
			files( { "../source/**.h*", "../source/**.c*" } )
			-- linker
			kind( "SharedLib" )
			links( { "coca", "irrlicht", "irrlicht_3rdparty", "gdi32", "opengl32", "winmm" } )
			
			configuration( "debug" )
				-- compiler
				defines( { "_DEBUG" } )
				flags( { "Symbols" } )
				-- linker
				targetdir( coca.premake.getTargetDir() )
				libdirs( { coca.premake.getCocaLibDir(), targetdir() } )
				-- post build
				postbuildcommands( { coca.premake.getCopyPluginCommand() } )
				
			configuration( "release" )
				-- compiler
				defines( { "NDEBUG" } )
				flags( { "Optimize" } )
				-- linker
				targetdir( coca.premake.getTargetDir() )
				libdirs( { coca.premake.getCocaLibDir(), targetdir() } )
				-- post build
				postbuildcommands( { coca.premake.getCopyPluginCommand() } )
		
		-- This defines a test project
		project( COCA_PROJECT_NAME.."-test" )
			location( toolset )
			-- compiler
			files( { "../test/**", "../source/**.h*", "../source/**.c*" } )
			-- linker
			kind( "ConsoleApp" )
			links( { "coca", "irrlicht", "irrlicht_3rdparty", "gdi32", "opengl32", "winmm" } )
			
			configuration( "debug" )
				-- compiler
				defines( { "_DEBUG" } )
				flags( { "Symbols" } )
				-- linker
				targetdir( coca.premake.getTargetDir() )
				libdirs( { coca.premake.getCocaLibDir(), targetdir() } )
				
			configuration( "release" )
				-- compiler
				defines( { "NDEBUG" } )
				flags( { "Optimize" } )
				-- linker
				targetdir( coca.premake.getTargetDir() )
				libdirs( { coca.premake.getCocaLibDir(), targetdir() } )
end
