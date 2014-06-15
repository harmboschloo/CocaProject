
function coca.writeHeader( file, guard, defInclude, namespace )
	file:write( "// This file was automatically generated! See premake scripts.\n" )
	file:write( "// Copyright (C) 2008-2009 Harm Boschloo\n\n" )
	file:write( "#ifndef "..guard.."\n" )
	file:write( "#define "..guard.."\n\n" )
	if defInclude ~= nil then file:write( defInclude ) end
	if namespace ~= nil then file:write( "namespace "..namespace.." {\n\n" ) end
end

function coca.writeFooter( file, guard, namespace )
	if namespace ~= nil then file:write( "} // namespace "..namespace.."" ) end
	file:write( "\n\n#endif // "..guard.."\n" )
end
