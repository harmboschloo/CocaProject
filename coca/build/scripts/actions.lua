newoption( {
	trigger    	= "script",
	value		= "string",
	description = "A script string to execute, see action 'execute'",
} )

newaction( {
	trigger     = "execute",
	description = "Executes a script, see option '--script'",
	execute = function()
		assert( _OPTIONS["script"], "no script option specified" )
		assert( loadstring( _OPTIONS["script"] ), "Error executing script" )()
	end
} )
