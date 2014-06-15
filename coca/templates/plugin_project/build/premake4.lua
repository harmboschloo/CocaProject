dofile( "config.lua" )
dofile( "scripts.lua" )
dofile( "projects.lua" )
loadLocalConfig( { "COCA_LOCATION" } )

function includeCocaScripts()
	dofile( path.join( COCA_LOCATION, "build/scripts.lua" ) )
end

if _ACTION == "clean" then
	for action in pairs( premake.actions ) do os.rmdir( action ) end
elseif premake.actions[_ACTION] then
	includeCocaScripts()
	defineProjects( _ACTION ) -- _ACTION specifies the toolset
else
	includeCocaScripts() -- defines additional actions
end
