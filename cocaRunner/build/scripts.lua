
function loadLocalConfig( definitions )
	assert( definitions )

	checkLocalConfigFilename()
	
	if os.isfile( LOCAL_CONFIG_FILENAME ) then
		dofile( LOCAL_CONFIG_FILENAME )
		for i,id in pairs( definitions ) do
			if not _G[id] or _G[id] == "" then _G[id] = os.getenv( id ) end
			assert( _G[id], id.." not found in "..LOCAL_CONFIG_FILENAME.." or in environment!" );
		end
	else
		local allSet = true
		local file = io.open ( LOCAL_CONFIG_FILENAME, "w" )
		file:write( "-- Please provide the following definition(s) in this file or in the environment.\n" )
		file:write( "-- Enter paths here like: \"D:/dev/coca\" or \"D:\\\\dev\\\\coca\". Don't use single forward slashes (\\)!\n" )
		file:write( "-- Definitions here overwrite environment definitions.\n" )
		for i,id in pairs( definitions ) do
			_G[id] = os.getenv( id )
			if _G[id] then 
				file:write( "-- "..id.." = \""..checkDefinition( _G[id] ).."\"\n" )
			else
				allSet = false;
				file:write( id.." = \"\"\n" )
			end
		end
		io.close( file )
		
		assert( allSet, "Not all definitions are specified! See "..LOCAL_CONFIG_FILENAME )
	end
end

function checkLocalConfigFilename()
	if _G["LOCAL_CONFIG_FILENAME"] then return end
	
	local userName = os.getenv( "USERNAME" )
	assert( userName, "No USERNAME found" )

	local userDomain = os.getenv( "USERDOMAIN" )
	assert( userDomain, "No USERDOMAIN found" )

	_G["LOCAL_CONFIG_FILENAME"] = "config." .. userName .. "." .. userDomain .. ".lua"
end

function checkDefinition( definition )
	local path = path.translate( definition, "/" )
	if os.isdir( path ) or os.isfile( path ) then return path 
	else return definition end
end
