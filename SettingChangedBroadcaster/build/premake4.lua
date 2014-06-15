dofile( "config.lua" )
dofile( "scripts.lua" )
dofile( "projects.lua" )
loadLocalConfig( { "COCA_LOCATION" } )
loadCocaScripts()

if _ACTION == "clean" then
	for action in pairs( premake.actions ) do os.rmdir( action ) end
elseif premake.actions[_ACTION] then
	defineProjects( _ACTION ) -- _ACTION specifies the toolset
end
