
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
		flags( { "ExtraWarnings", "WinMain" } )
		includedirs( { coca.getIncludePath(), path.join( WX_WIDGETS_LOCATION, "include" ), "../vendor/wxpropgrid/include" } )
		resincludedirs ( { path.join( WX_WIDGETS_LOCATION, "include" ) } )
		defines( { "__WXMSW__", "WIN32", "__WIN32__", "_WINDOWS", "NOPCH", "WXMAKINGLIB_PROPGRID" } )
		if string.sub( toolset, 1, 2 ) == "vs" then
			includedirs( { path.join( WX_WIDGETS_LOCATION, "include/msvc" ) } )
			libdirs( { path.join( WX_WIDGETS_LOCATION, "lib/vc_lib" ) } )
		elseif toolset == "codeblocks" then
			defines( { "__GNUWIN32__" } )
			libdirs( { path.join( WX_WIDGETS_LOCATION, "lib/gcc_lib" ) } )
		else
			assert( false )
		end
		-- linker
		links( { "coca" } )
		local osLinks = {}
		if os.is( "windows" ) then
			osLinks = { "rpcrt4", "ole32", "oleaut32", "uuid", "comctl32", "winspool"  }
		else
			assert( false )
		end

		-- This defines the project
	   project( COCA_PROJECT_NAME )
			location( toolset )
			-- compiler
			files( { "../source/**.h*", "../source/**.c*", "../source/**.xpm*", "../vendor/wxpropgrid/include/**.h*", "../vendor/wxpropgrid/src/**.c*" } )
			excludes( { "../vendor/wxpropgrid/**xh_propgrid*" } )
			-- linker
			kind( "WindowedApp" )
			
			configuration( "debug" )
				-- compiler
				defines( { "_DEBUG", "__WXDEBUG__" } )
				flags( { "Symbols" } )
				if toolset == "codeblocks" then
					includedirs( { path.join( WX_WIDGETS_LOCATION, "lib/gcc_lib/mswd" ) } )
				end
				-- linker
				targetdir( coca.premake.getTargetDir() )
				libdirs( { coca.premake.getCocaLibDir() } )
				--links( { "wxmsw28d" } )
				links( { "wxmsw28d_aui", "wxmsw28d_adv", "wxmsw28d_core", "wxbase28d" } )
				--links( { "wxexpatd", "wxregexd", "wxzlibd", "wxpngd", "wxjpegd", "wxtiffd" } ) -- order?
				links( osLinks ) -- after wx links!
				if string.sub( toolset, 1, 2 ) == "vs" then
					linkoptions( { "/NODEFAULTLIB:LIBCMTD" } )
				end			
				-- post build
				postbuildcommands( { coca.premake.getCopyToolCommand() } )

			configuration( "release" )
				-- compiler
				defines( { "NDEBUG" } )
				flags( { "Optimize" } )
				if toolset == "codeblocks" then
					includedirs( { path.join( WX_WIDGETS_LOCATION, "lib/gcc_lib/msw" ) } )
				end
				-- linker
				targetdir( coca.premake.getTargetDir() )
				libdirs( { coca.premake.getCocaLibDir() } )
				--links( { "wxmsw28" } )
				--links( { "wxbase28", "wxbase28_xml", "wxmsw28_core", "wxmsw28_adv", "wxmsw28_html", "wxmsw28_aui" } ) -- wrong order!
				--links( { "wxexpat", "wxregex", "wxzlib", "wxpng", "wxjpeg", "wxtiff" } ) 
				links( { "wxmsw28_aui", "wxmsw28_adv", "wxmsw28_core", "wxbase28" } )
				links( osLinks ) -- after wx links!
				if string.sub( toolset, 1, 2 ) == "vs" then
					linkoptions( { "/NODEFAULTLIB:LIBCMT" } )
				end
				-- post build
				postbuildcommands( { coca.premake.getCopyToolCommand() } )

end
