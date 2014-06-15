
function coca.generateAlgorithmFiles( maxLevel, destination, namespace ) -- namespace is optional
	os.rmdir( destination )
	coca.generateForAllFile( maxLevel, destination, namespace )
	coca.generateForEachFile( maxLevel, destination, namespace )
end

-- forAll file --

function coca.generateForAllFile( maxLevel, destination, namespace )
	local tab = "    "
	local filename = path.join( destination, "forAll.h" )
	local file, err = io.open( filename, "w" )
	assert( file, err )
	local guard = "AGEN_FOR_ALL_H_INCLUDED"
	if namespace then guard = string.upper( namespace ).."_"..guard end

	-- header
	coca.writeHeader( file, guard, "", namespace )

	-- forAll
	local level = 0
	while level <= maxLevel do
		file:write( "template<typename Container, typename Function" )
		coca.repeatCountedWrite( file, level, ", typename Argument" )
		file:write( ">\n" )
		file:write( "Function forAll( const Container& container, Function function" )
		coca.repeatCountedWrite( file, level, ", Argument", "& a" )
		file:write( " )\n{\n" )
		file:write( tab.."typename Container::const_iterator it;\n" )
		file:write( tab.."for ( it = container.begin(); it != container.end(); ++it )\n" )
		file:write( tab.."{\n" )
		file:write( tab..tab.."function( *it" )
		coca.repeatCountedWrite( file, level, ", a" )
		file:write( " );\n" )
		file:write( tab.."}\n" )
		file:write( tab.."return function;\n}\n\n" )
		level = level+1
	end	
	
	-- footer
	coca.writeFooter( file, guard, namespace )

	io.close( file )
end

-- forEach file --

function coca.generateForEachFile( maxLevel, destination, namespace )
	local tab = "    "
	local filename = path.join( destination, "forEach.h" )
	local file, err = io.open( filename, "w" )
	assert( file, err )
	local guard = "AGEN_FOR_EACH_H_INCLUDED"
	if namespace then guard = string.upper( namespace ).."_"..guard end

	-- header
	coca.writeHeader( file, guard, "", namespace )

	-- forEach
	local level = 0
	while level <= maxLevel do
		file:write( "template<typename Iterator, typename Function" )
		coca.repeatCountedWrite( file, level, ", typename Argument" )
		file:write( ">\n" )
		file:write( "Function forEach( Iterator begin, Iterator end, Function function" )
		coca.repeatCountedWrite( file, level, ", Argument", "& a" )
		file:write( " )\n{\n" )
		file:write( tab.."for ( ; begin != end; ++begin )\n" )
		file:write( tab.."{\n" )
		file:write( tab..tab.."function( *begin" )
		coca.repeatCountedWrite( file, level, ", a" )
		file:write( " );\n" )
		file:write( tab.."}\n" )
		file:write( tab.."return function;\n}\n\n" )
		level = level+1
	end	
	
	-- footer
	coca.writeFooter( file, guard, namespace )

	io.close( file )
end
