
function defineProjects( toolset )
	-- indents are purly cosmetic
	-- A solution contains projects, and defines the available configurations
	solution( "coca" )
		location( toolset )
		configurations(  { "debug", "release" } )
		-- compiler
		language( "C++" )
		flags( { "ExtraWarnings" } )
		includedirs( { "../include" } )

		configuration( "debug" )
			-- compiler
			defines( { "DEBUG" } )
			flags( { "Symbols" } )
			-- linker
			targetdir( coca.premake.getTargetDir() )
			libdirs( { targetdir() } )

		configuration( "release" )
			-- compiler
			defines( { "NDEBUG" } )
			flags( { "Optimize" } )
			-- linker
			targetdir( coca.premake.getTargetDir() )
			libdirs( { targetdir() } )

	   -- A project defines one build target
	   project( "coca" )
			location( toolset )
			-- compiler
			files( { "../include/coca/**", "../source/**" } )
			defines( { "COCA_DLL_BUILD" } )
			--flags( { "NoRTTI" } )
			-- linker
			kind( "SharedLib" )

		project( "coca-test" )
			location( toolset )
			-- compiler
			files( { "../test/**" } )
			-- linker
			kind( "ConsoleApp" )
			links( { "coca" } )
end
