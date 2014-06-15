assert( coca == nil )
coca = {}

function coca.getProjectName()
	assert( COCA_PROJECT_NAME )
	return COCA_PROJECT_NAME
end

function coca.getLocation()
	assert( COCA_LOCATION )
	return COCA_LOCATION
end

function coca.getIncludePath()
	assert( COCA_LOCATION )
	return path.join( COCA_LOCATION, "include" ) 
end

function coca.getInterfacesPath()
	return path.join( coca.getIncludePath(), "coca/interfaces" ) 
end

function coca.translateToLocalPath( p )
	if ( os.is( "windows" ) ) then
		return path.translate( p, "\\" )
	elseif ( os.is( "linux" ) ) then
		return path.translate( p, "/" )
	else
		error( "undefined, please specify or report!" )
	end
end

function coca.getCopyInterfaceCommand( interfaceFile )
	return coca.getCopyCommand( interfaceFile, coca.getInterfacesPath() )
end

function coca.getCopyCommand( sourceFile, destinationDirectory )
	local command = ""
	if ( os.is( "windows" ) ) then
		command = path.join( coca.getLocation(), "build/scripts/copy.bat" )
	else
		error( "undefined, please specify or report!" )
	end
	
	return "\""..coca.translateToLocalPath( command ).."\" \""..coca.translateToLocalPath( sourceFile ).."\" \""..coca.translateToLocalPath( destinationDirectory ).."\""
end

function coca.getDllFilename( dllName )
	if ( os.is( "windows" ) ) then
		return dllName..".dll"
	elseif ( os.is( "linux" ) ) then
		return dllName..".so"
	else
		error( "undefined, please specify or report!" )
	end
end

function coca.getApplicationFilename( applicationName )
	if ( os.is( "windows" ) ) then
		return applicationName..".exe"
	else
		error( "undefined, please specify or report!" )
	end
end
