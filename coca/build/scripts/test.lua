function coca.generateIncludeTestFiles( headerFiles, includeDir, destinationDir )
	-- remove old generated files first
	os.rmdir( destinationDir )

	-- generate include test files
	local matches = os.matchfiles( headerFiles )
	print( "Generating include test files (" .. #matches .. ")" )
	for k,v in pairs( matches ) do
		local headerFile = path.getrelative( includeDir, v )
		local testFile = path.join( destinationDir, path.getbasename( headerFile ).. ".cpp" )
		local file = io.open( testFile, "w" );
		file:write( "#include <" .. headerFile .. ">\n" )
		io.close( file )
	end
end
