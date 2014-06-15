
function coca.generateFunctionFiles( maxLevel, destination, namespace ) -- namespace is optional
	os.rmdir( destination )
	coca.generateRefMemberFunctionFiles( maxLevel, destination, namespace )
	coca.generatePtrMemberFunctionFiles( maxLevel, destination, namespace )
	coca.generateObjectFunctionFiles( maxLevel, destination, namespace )
	coca.generateFunctionAdaptorInterfaceFiles( maxLevel, destination, namespace )
	coca.generateObjectFunctionAdaptorFiles( maxLevel, destination, namespace )
	coca.generateFunctionWrapperFiles( maxLevel, destination, namespace )
	coca.generateFunctionHeaderFile( maxLevel, destination, namespace )
end

-- Member function files --

function coca.generateRefMemberFunctionFiles( maxLevel, destination, namespace )
	print( "Generating ref member function files (up to level "..maxLevel..")" )
	local level = 0
	while level <= maxLevel do
		coca.generateRefMemberFunctionFile( level, maxLevel, destination, namespace )
		level = level+1
	end
end

function coca.generateRefMemberFunctionFile( level, maxLevel, destination, namespace )
	local className = "RefMemberFunction"
	local filename = destination.."/"..className..level..".h"
	local file, err = io.open( filename, "w" )
	assert( file, err )
	local guard = "AGEN_REF_MEMBER_FUNCTION_"..level.."_H_INCLUDED"
	if namespace ~= nil then guard = string.upper( namespace ).."_"..guard end

	-- header
	if level == maxLevel then
		coca.writeHeader( file, guard, "", namespace )
	else
		coca.writeHeader( file, guard, "#include \""..className..maxLevel..".h\"\n\n", namespace )
	end
	
	-- class definitions
	coca.writeMemberFunctionClass( file, level, maxLevel, className, "&", "Return", "" )
	file:write( "// void partial specialization\n\n" )
	coca.writeMemberFunctionClass( file, level, maxLevel, className, "&", "void", "" )
	file:write( "// const partial specialization\n\n" )
	coca.writeMemberFunctionClass( file, level, maxLevel, className, "&", "Return", "const " )
	file:write( "// void const partial specialization\n\n" )
	coca.writeMemberFunctionClass( file, level, maxLevel, className, "&", "void", "const " )
	
	-- make functions
	file:write( "// make functions\n\n" )
	coca.writeMemberFunctionMakeFunction( file, level, className, "&", "" )
	coca.writeMemberFunctionMakeFunction( file, level, className, "&", "const " )

	-- footer
	coca.writeFooter( file, guard, namespace )

	io.close( file )
end

function coca.generatePtrMemberFunctionFiles( maxLevel, destination, namespace )
	print( "Generating ptr member function files (up to level "..maxLevel..")" )

	local level = 0
	while level <= maxLevel do
		coca.generatePtrMemberFunctionFile( level, maxLevel, destination, namespace )
		level = level+1
	end
end

function coca.generatePtrMemberFunctionFile( level, maxLevel, destination, namespace )
	local className = "PtrMemberFunction"
	local filename = destination.."/"..className..level..".h"
	local file, err = io.open( filename, "w" )
	assert( file, err )
	local guard = "AGEN_PTR_MEMBER_FUNCTION_"..level.."_H_INCLUDED"
	if namespace ~= nil then guard = string.upper( namespace ).."_"..guard end
	
	-- header
	if level == maxLevel then
		coca.writeHeader( file, guard, "", namespace )
	else
		coca.writeHeader( file, guard, "#include \""..className..maxLevel..".h\"\n\n", namespace )
	end
	
	-- class definitions
	coca.writeMemberFunctionClass( file, level, maxLevel, className, "*", "Return", "" )
	file:write( "// void partial specialization\n\n" )
	coca.writeMemberFunctionClass( file, level, maxLevel, className, "*", "void", "" )
	file:write( "// const partial specialization\n\n" )
	coca.writeMemberFunctionClass( file, level, maxLevel, className, "*", "Return", "const " )
	file:write( "// void const partial specialization\n\n" )
	coca.writeMemberFunctionClass( file, level, maxLevel, className, "*", "void", "const " )
	
	-- make functions
	file:write( "// make functions\n\n" )
	coca.writeMemberFunctionMakeFunction( file, level, className, "*", "" )
	coca.writeMemberFunctionMakeFunction( file, level, className, "*", "const " )

	-- footer
	coca.writeFooter( file, guard, namespace )

	io.close( file )
end

function coca.writeMemberFunctionClass( file, level, maxLevel, className, objectType, returnType, constValue )
	local tab = "    "
	local accessor = ""
	if objectType == "*" then
		accessor = "->"
	else
		accessor = "."
	end
	
	-- template definition
	file:write( "template<" ) 
	if returnType == "Return" then
		file:write( "typename Return, " )
	end
	file:write( "typename Class" )
	if level == maxLevel and returnType == "Return" and constValue == "" then
		coca.repeatCountedWrite( file, level, ", typename Argument", nil, " = void" )
	else
		coca.repeatCountedWrite( file, level, ", typename Argument" )
	end
	file:write( ">\n" )
	
	-- class definition
	file:write( "class "..className )
	if level ~= maxLevel or returnType ~= "Return" or constValue ~= "" then
		file:write( "<"..returnType..","..constValue.."Class" )
		coca.repeatCountedWrite( file, level, ",Argument" )
		coca.repeatWrite( file, ( maxLevel-level ), ",void" )
		file:write( ">" )
	end
	file:write( "\n{\npublic:\n" )
	
	-- typedef
	file:write( tab.."typedef "..returnType.." ( Class::*MemberFunctionPointer )( " )
	coca.repeatCountedWriteSkipLast( file, level, "Argument", nil, ", " )
	file:write( " ) "..constValue..";\n\n" )
	
	-- constructor
	file:write( tab..className.."( MemberFunctionPointer member ) : _member( member ) {}\n\n" )
	
	-- operator
	file:write( tab..returnType.." operator()( "..constValue.."Class"..objectType.." object" )
	coca.repeatCountedWrite( file, level, ", Argument", " argument" )
	file:write( " ) const\n" )
	file:write( tab.."{\n"..tab..tab ) 
	if returnType ~= "void" then file:write( "return " ) end
	file:write( "( object"..accessor.."*_member )( " )
	coca.repeatCountedWriteSkipLast( file, level, "argument", nil, ", " )
	file:write( " );\n"..tab.."}\n\n" )
	
	-- data and end
	file:write( "private:\n"..tab.."MemberFunctionPointer _member;\n};\n\n" )
end

function coca.writeMemberFunctionMakeFunction( file, level, className, objectType, constValue )
	local tab = "    "
	file:write( "template<typename Return, typename Class" )
	coca.repeatCountedWrite( file, level, ", typename Argument" )
	file:write( ">\n" )
	file:write( "inline "..className.."<Return,"..constValue.."Class" )
	coca.repeatCountedWrite( file, level, ",Argument" )
	file:write( "> make"..className.."(\n" )
	file:write( tab.."Return ( Class::*memberPointer )( " )
	coca.repeatCountedWriteSkipLast( file, level, "Argument", nil, ", " )
	file:write( " ) "..constValue..")\n{\n" )
	file:write( tab.."return "..className.."<Return,"..constValue.."Class" )
	coca.repeatCountedWrite( file, level, ",Argument" )
	file:write( ">( memberPointer );\n}\n\n" )
end

-- Object function --

function coca.generateObjectFunctionFiles( maxLevel, destination, namespace )
	print( "Generating object function files (up to level "..maxLevel..")" )

	local level = 0
	while level <= maxLevel do
		coca.generateObjectFunctionFile( level, maxLevel, destination, namespace )
		level = level+1
	end
end

function coca.generateObjectFunctionFile( level, maxLevel, destination, namespace )
	local filename = destination.."/ObjectFunction"..level..".h"
	local file, err = io.open( filename, "w" )
	assert( file, err )
	local guard = "AGEN_OBJECT_FUNCTION_"..level.."_H_INCLUDED"
	if namespace ~= nil then guard = string.upper( namespace ).."_"..guard end
	
	-- header
	if level == maxLevel then
		coca.writeHeader( file, guard, "#include \"RefMemberFunction"..level..".h\"\n\n", namespace )
	else
		coca.writeHeader( file, guard, "#include \"ObjectFunction"..maxLevel..".h\"\n#include \"RefMemberFunction"..level..".h\"\n\n", namespace )
	end

	-- class definitions
	coca.writeObjectFunctionClass( file, level, maxLevel, "Return" )
	file:write( "// void partial specialization\n\n" )
	coca.writeObjectFunctionClass( file, level, maxLevel, "void" )

	-- make functions
	file:write( "// make functions\n\n" )
	coca.writeObjectFunctionMakeFunction( file, level, "" )
	coca.writeObjectFunctionMakeFunction( file, level, "const " )

	-- footer
	coca.writeFooter( file, guard, namespace )

	io.close( file )
end

function coca.writeObjectFunctionClass( file, level, maxLevel, returnType )
	local tab = "    "
	
	-- template definition
	file:write( "template<" ) 
	if returnType == "Return" then file:write( "typename Return, " ) end
	file:write( "typename Class" )
	if level == maxLevel and returnType == "Return" then
		coca.repeatCountedWrite( file, level, ", typename Argument", nil, " = void" )
		file:write( ", typename MemberFunction = RefMemberFunction<"..returnType..",Class" )
		coca.repeatCountedWrite( file, level, ",Argument" )
		file:write( "> " )
	else
		coca.repeatCountedWrite( file, level, ", typename Argument" )
		file:write( ", typename MemberFunction" )
	end
	file:write( ">\n" )
	
	-- class definition
	file:write( "class ObjectFunction" )
	if level ~= maxLevel or returnType ~= "Return" then
		file:write( "<"..returnType..",Class" )
		coca.repeatCountedWrite( file, level, ",Argument" )
		coca.repeatWrite( file, ( maxLevel-level ), ",void" )
		file:write( ",MemberFunction>" )
	end
	file:write( "\n{\npublic:\n" )
	
	-- constructor
	file:write( tab.."ObjectFunction( Class& object, const MemberFunction& memberFunction ) : \n" )
	file:write( tab..tab..tab.."_object( &object ), _memberFunction( memberFunction ) {}\n\n" )
	
	-- operator
	file:write( tab..returnType.." operator()( " )
	coca.repeatCountedWriteSkipLast( file, level, "Argument", " argument", ", " )
	file:write( " ) const\n" )
	file:write( tab.."{\n"..tab..tab ) 
	if returnType ~= "void" then file:write( "return " ) end
	file:write( "_memberFunction( *_object" )
	coca.repeatCountedWrite( file, level, ", argument" )
	file:write( " );\n"..tab.."}\n\n" )

	-- data and end
	file:write( "private:\n"..tab.."Class* _object;\n"..tab.."MemberFunction _memberFunction;\n};\n\n" )
end

function coca.writeObjectFunctionMakeFunction( file, level, constValue )
	local tab = "    "
	file:write( "template<typename Return, typename Class" )
	coca.repeatCountedWrite( file, level, ", typename Argument" )
	file:write( ">\n" )
	file:write( "inline ObjectFunction<Return,"..constValue.."Class" )
	coca.repeatCountedWrite( file, level, ",Argument" )
	file:write( "> makeObjectFunction(\n"..tab..constValue.."Class& object, Return ( Class::*memberPointer )( " )
	coca.repeatCountedWriteSkipLast( file, level, "Argument", nil, ", " )
	file:write( " ) "..constValue..")\n{\n" )
	file:write( tab.."return ObjectFunction<Return,"..constValue.."Class" )
	coca.repeatCountedWrite( file, level, ",Argument" )
	file:write( ">( object, makeRefMemberFunction( memberPointer ) );\n}\n\n" )
end

-- Function adaptor interface --

function coca.generateFunctionAdaptorInterfaceFiles( maxLevel, destination, namespace )
	print( "Generating function adaptor interface files (up to level "..maxLevel..")" )
	local level = 0
	while level <= maxLevel do
		coca.generateFunctionAdaptorInterfaceFile( level, maxLevel, destination, namespace )
		level = level+1
	end
end

function coca.generateFunctionAdaptorInterfaceFile( level, maxLevel, destination, namespace )
	local filename = destination.."/IFunctionAdaptor"..level..".h"
	local file, err = io.open( filename, "w" )
	assert( file, err )
	local guard = "AGEN_I_FUNCTION_ADAPTOR_"..level.."_H_INCLUDED"
	if namespace ~= nil then guard = string.upper( namespace ).."_"..guard end

	-- header
	if level == maxLevel then
		coca.writeHeader( file, guard, "#include \"../../ReferenceCounted.h\"\n\n", namespace )
	else
		coca.writeHeader( file, guard, "#include \"IFunctionAdaptor"..maxLevel..".h\"\n#include \"../../ReferenceCounted.h\"\n\n", namespace )
	end
	
	-- class definitions
	coca.writeIFunctionAdaptorClass( file, level, maxLevel, "Return" )
	if level == 0 then
		coca.writeIFunctionAdaptorClass( file, level, maxLevel, "void" )
	end
	
	-- footer
	coca.writeFooter( file, guard, namespace )

	io.close( file )
end

function coca.writeIFunctionAdaptorClass( file, level, maxLevel, returnType )
	local tab = "    "
	
	-- template definition
	if level == maxLevel then
		file:write( "template<typename Return = void" ) 
		coca.repeatCountedWrite( file, level, ", typename Argument", nil, " = void" )
	else
		if returnType == "Return" then
			file:write( "template<typename Return" )
			coca.repeatCountedWrite( file, level, ", typename Argument" )
		else
			file:write( "template<" )
			coca.repeatCountedWriteSkipLast( file, level, "typename Argument", nil, ", " )
		end
	end
	file:write( ">\n" )
	
	-- class definition
	file:write( "class IFunctionAdaptor" )
	if level ~= maxLevel then
		file:write( "<"..returnType )
		coca.repeatCountedWrite( file, level, ",Argument" )
		coca.repeatWrite( file, ( maxLevel-level ), ",void" )
		file:write( ">" )
	end
	file:write( " : public ReferenceCounted\n{\npublic:\n" )

	-- destructor
	file:write( tab.."virtual ~IFunctionAdaptor() {}\n" )
	
	-- operator
	file:write( tab.."virtual "..returnType.." operator()( " )
	coca.repeatCountedWriteSkipLast( file, level, "Argument", " argument", ", " )
	file:write( " ) const = 0;\n" )	
	
	-- end
	file:write( "};\n\n" )
end

-- Object function adaptor --

function coca.generateObjectFunctionAdaptorFiles( maxLevel, destination, namespace )
	print( "Generating object function adaptor files (up to level "..maxLevel..")" )
	local level = 0
	while level <= maxLevel do
		coca.generateObjectFunctionAdaptorFile( level, maxLevel, destination, namespace )
		level = level+1
	end
end

function coca.generateObjectFunctionAdaptorFile( level, maxLevel, destination, namespace )
	local filename = destination.."/ObjectFunctionAdaptor"..level..".h"
	local file, err = io.open( filename, "w" )
	assert( file, err )
	local guard = "AGEN_FUNCTION_ADAPTOR_"..level.."_H_INCLUDED"
	if namespace ~= nil then guard = string.upper( namespace ).."_"..guard end

	-- header
	includes = "#include \"IFunctionAdaptor"..level..".h\"\n#include \"ObjectFunction"..level..".h\"\n"
	if level == maxLevel then
		coca.writeHeader( file, guard, includes.."\n", namespace )
	else
		coca.writeHeader( file, guard, "#include \"ObjectFunctionAdaptor"..maxLevel..".h\"\n"..includes.."\n", namespace )
	end
	
	-- class definitions
	coca.writeObjectFunctionAdaptorClass( file, level, maxLevel, "Return" )
	file:write( "// void partial specialization\n\n" )
	coca.writeObjectFunctionAdaptorClass( file, level, maxLevel, "void" )
	
	-- create functions
	file:write( "// create functions\n\n" )
	coca.writeObjectFunctionAdaptorCreateFunction( file, level, maxLevel,  "" )
	coca.writeObjectFunctionAdaptorCreateFunction( file, level, maxLevel, "const " )

	-- footer
	coca.writeFooter( file, guard, namespace )

	io.close( file )
end

function coca.writeObjectFunctionAdaptorClass( file, level, maxLevel, returnType )
	local tab = "    "
	
	-- template definition
	file:write( "template<" ) 
	if returnType == "Return" then file:write( "typename Return, " ) end
	file:write( "typename Class" )
	if level == maxLevel and returnType == "Return" then
		coca.repeatCountedWrite( file, level, ", typename Argument", nil, " = void" )
		file:write( ", typename ObjectFunctionType = ObjectFunction<"..returnType..",Class" )
		coca.repeatCountedWrite( file, level, ",Argument" )
		file:write( "> " )
	else
		coca.repeatCountedWrite( file, level, ", typename Argument" )
		file:write( ", typename ObjectFunctionType" )
	end
	file:write( ">\n" )
	
	-- class definition
	file:write( "class ObjectFunctionAdaptor" )
	if level ~= maxLevel or returnType ~= "Return" then
		file:write( "<"..returnType..",Class" )
		coca.repeatCountedWrite( file, level, ",Argument" )
		coca.repeatWrite( file, ( maxLevel-level ), ",void" )
		file:write( ",ObjectFunctionType>" )
	end
	file:write( " : public IFunctionAdaptor<"..returnType )
	coca.repeatCountedWrite( file, level, ",Argument" )
	file:write( ">\n{\npublic:\n" )

	-- constructor
	file:write( tab.."ObjectFunctionAdaptor( const ObjectFunctionType& objectFunction ) : \n" )
	file:write( tab..tab..tab.."_objectFunction( objectFunction ) {}\n\n" )

	-- operator
	file:write( tab..returnType.." operator()( " )
	coca.repeatCountedWriteSkipLast( file, level, "Argument", " argument", ", " )
	file:write( " ) const\n" )
	file:write( tab.."{\n"..tab..tab ) 
	if returnType ~= "void" then file:write( "return " ) end
	file:write( "_objectFunction( " )
	coca.repeatCountedWriteSkipLast( file, level, "argument", nil, ", " )
	file:write( " );\n"..tab.."}\n\n" )
	
	-- data and end
	file:write( "private:\n"..tab.."ObjectFunctionType _objectFunction;\n};\n\n" )
end

function coca.writeObjectFunctionAdaptorCreateFunction( file, level, maxLevel, constValue )
	local tab = "    "
	file:write( "template<typename Return, typename Class" )
	coca.repeatCountedWrite( file, level, ", typename Argument" )
	file:write( ">\n" )
	file:write( "inline ObjectFunctionAdaptor<Return,"..constValue.."Class" )
	coca.repeatCountedWrite( file, level, ",Argument" )
	file:write( ">* createObjectFunctionAdaptor(\n"..tab..constValue.."Class& object, Return ( Class::*memberPointer )( " )
	coca.repeatCountedWriteSkipLast( file, level, "Argument", nil, ", " )
	file:write( " ) "..constValue..")\n{\n" )
	file:write( tab.."return new ObjectFunctionAdaptor<Return,"..constValue.."Class" )
	coca.repeatCountedWrite( file, level, ",Argument" )
	file:write( ">( makeObjectFunction( object, memberPointer ) );\n}\n\n" )
	
	file:write( "template<typename FunctionReturn" )
	coca.repeatCountedWrite( file, level, ", typename FunctionArgument" )
	file:write( ", typename Return, typename Class" )
	coca.repeatCountedWrite( file, level, ", typename Argument" )
	file:write( ">\n" )
	file:write( "inline ObjectFunctionAdaptor<FunctionReturn,"..constValue.."Class" )
	coca.repeatCountedWrite( file, level, ",FunctionArgument" )
	coca.repeatWrite( file, (maxLevel-level), ",void" )
	file:write( ",ObjectFunction<Return,"..constValue.."Class" )
	coca.repeatCountedWrite( file, level, ",Argument" )
	file:write( "> >* createObjectFunctionAdaptor(\n"..tab..constValue.."Class& object, Return ( Class::*memberPointer )( " )
	coca.repeatCountedWriteSkipLast( file, level, "Argument", nil, ", " )
	file:write( " ) "..constValue..")\n{\n" )
	file:write( tab.."return new ObjectFunctionAdaptor<FunctionReturn,"..constValue.."Class" )
	coca.repeatCountedWrite( file, level, ",FunctionArgument" )
	coca.repeatWrite( file, (maxLevel-level), ",void" )
	file:write( ",ObjectFunction<Return,"..constValue.."Class" )
	coca.repeatCountedWrite( file, level, ",Argument" )
	file:write( "> >( makeObjectFunction( object, memberPointer ) );\n}\n\n" )
end

-- Function --

function coca.generateFunctionWrapperFiles( maxLevel, destination, namespace )
	print( "Generating function files (up to level "..maxLevel..")" )

	local level = 0
	while level <= maxLevel do
		coca.generateFunctionFile( level, maxLevel, destination, namespace )
		level = level+1
	end
end

function coca.generateFunctionFile( level, maxLevel, destination, namespace )
	local filename = destination.."/Function"..level..".h"
	local file, err = io.open( filename, "w" )
	assert( file, err )
	local guard = "AGEN_FUNCTION_"..level.."_H_INCLUDED"
	if namespace ~= nil then guard = string.upper( namespace ).."_"..guard end

	-- header
	includes = "#include \"ObjectFunctionAdaptor"..level..".h\"\n#include \"../../ReferencePointer.h\"\n#include \"../../test.h\"\n"
	if level == maxLevel then
		coca.writeHeader( file, guard, includes.."\n", namespace )
	else
		coca.writeHeader( file, guard, "#include \"Function"..maxLevel..".h\"\n"..includes.."\n", namespace )
	end
		
	-- class definitions
	coca.writeFunctionClass( file, level, maxLevel, "Return" )
	file:write( "// void partial specialization\n\n" )
	coca.writeFunctionClass( file, level, maxLevel, "void" )
	
	-- create functions
	file:write( "// make functions\n\n" )
	coca.writeFunctionMakeFunction( file, level, "" )
	coca.writeFunctionMakeFunction( file, level, "const " )

	-- footer
	coca.writeFooter( file, guard, namespace )

	io.close( file )
end

function coca.writeFunctionClass( file, level, maxLevel, returnType )
	local tab = "    "
	
	-- template definition
	file:write( "template<" ) 
	if level == maxLevel and returnType == "Return" then
		file:write( "typename Return = void" )
		coca.repeatCountedWrite( file, level, ", typename Argument", nil, " = void" )
	else
		if returnType == "Return" then 
			file:write( "typename Return" )
			if level > 0 then file:write( ", " ) end
		end
		coca.repeatCountedWriteSkipLast( file, level, "typename Argument", nil, ", " )
	end
	file:write( ">\n" )
		
	-- class definition
	file:write( "class Function" )
	if level ~= maxLevel or returnType ~= "Return" then
		file:write( "<"..returnType.."" )
		coca.repeatCountedWrite( file, level, ",Argument" )
		coca.repeatWrite( file, ( maxLevel-level ), ",void" )
		file:write( ">" )
	end
	file:write( "\n{\npublic:\n" )
	
	-- typedefs
	file:write( tab.."typedef Function<"..returnType )
	coca.repeatCountedWrite( file, level, ",Argument" )
	file:write( "> FunctionType;\n" )
	file:write( tab.."typedef IFunctionAdaptor<"..returnType )
	coca.repeatCountedWrite( file, level, ",Argument" )
	file:write( "> FunctionAdaptorType;\n\n" )
	
	-- constructor
	file:write( tab.."Function( const FunctionAdaptorType* adaptor ) : _adaptor( adaptor ) {}\n\n" )
	
	-- ObjectFunction template constructor
	file:write( tab.."template<typename Class>\n" )
	file:write( tab.."Function( Class& object, "..returnType.." ( Class::*memberPointer )( " )
	coca.repeatCountedWriteSkipLast( file, level, "Argument", nil, ", " )
	file:write( " ) ) : \n"..tab..tab..tab.."_adaptor( createObjectFunctionAdaptor( object, memberPointer ) ) {}\n\n" )
	
	-- ObjectFunction template constructor with different types
	file:write( tab.."template<typename ActualReturn, typename Class" )
	coca.repeatCountedWrite( file, level, ", typename ActualArgument" )
	file:write( ">\n" )
	file:write( tab.."Function( Class& object, ActualReturn ( Class::*memberPointer )( " )
	coca.repeatCountedWriteSkipLast( file, level, "ActualArgument", nil, ", " )
	file:write( " ) ) : \n"..tab..tab..tab.."_adaptor( createObjectFunctionAdaptor<"..returnType )
	coca.repeatCountedWrite( file, level, ",Argument" )
	file:write( ">( object, memberPointer ) ) {}\n\n" )
	
	-- ObjectFunction template constructor for const objects
	file:write( tab.."template<typename Class>\n" )
	file:write( tab.."Function( const Class& object, "..returnType.." ( Class::*memberPointer )( " )
	coca.repeatCountedWriteSkipLast( file, level, "Argument", nil, ", " )
	file:write( " ) const ) : \n"..tab..tab..tab.."_adaptor( createObjectFunctionAdaptor( object, memberPointer ) ) {}\n\n" )
	
	-- ObjectFunction template constructor with different types for const objects
	file:write( tab.."template<typename ActualReturn, typename Class" )
	coca.repeatCountedWrite( file, level, ", typename ActualArgument" )
	file:write( ">\n" )
	file:write( tab.."Function( const Class& object, ActualReturn ( Class::*memberPointer )( " )
	coca.repeatCountedWriteSkipLast( file, level, "ActualArgument", nil, ", " )
	file:write( " ) const ) : \n"..tab..tab..tab.."_adaptor( createObjectFunctionAdaptor<"..returnType.."" )
	coca.repeatCountedWrite( file, level, ",Argument" )
	file:write( ">( object, memberPointer ) ) {}\n\n" )
	
	-- copy constructor
	file:write( tab.."Function( const FunctionType& other ) : _adaptor( other._adaptor ) {}\n\n" )
	
	-- asignment operator
	file:write( tab.."FunctionType& operator=( const FunctionType& other )\n"..tab.."{\n" )
	file:write( tab..tab.."_adaptor = other._adaptor;\n"..tab..tab.."return *this;\n"..tab.."}\n\n" )

	-- comparison operators
	file:write( tab.."inline bool operator==( const FunctionType& other ) const\n"..tab.."{\n" )
	file:write( tab..tab.."return ( _adaptor == other._adaptor );\n"..tab.."}\n\n" )
	
	file:write( tab.."inline bool operator!=( const FunctionType& other ) const\n"..tab.."{\n" )
	file:write( tab..tab.."return !( other==*this );\n"..tab.."}\n\n" )
	
	-- operator
	file:write( tab..returnType.." operator()( " )
	coca.repeatCountedWriteSkipLast( file, level, "Argument", " argument", ", " )
	file:write( " ) const\n" )
	file:write( tab.."{\n" ) 
	file:write( tab..tab.."COCA_ASSERT( _adaptor )\n"..tab..tab ) 
	if returnType ~= "void" then file:write( "return " ) end
	file:write( "( *_adaptor )( " )
	coca.repeatCountedWriteSkipLast( file, level, "argument", nil, ", " )
	file:write( " );\n"..tab.."}\n\n" )

	-- adapter accessor --
	file:write( tab.."const FunctionAdaptorType* getAdaptor() const\n"..tab.."{\n" )
	file:write( tab..tab.."return _adaptor;\n"..tab.."}\n\n" )

	-- data and end
	file:write( "private:\n"..tab.."ReferencePointer<const FunctionAdaptorType> _adaptor;\n};\n\n" )
end

function coca.writeFunctionMakeFunction( file, level, constValue )
	local tab = "    "
	file:write( "template<typename Return, typename Class" )
	coca.repeatCountedWrite( file, level, ", typename Argument" )
	file:write( ">\n" )
	file:write( "inline Function<Return" )
	coca.repeatCountedWrite( file, level, ",Argument" )
	file:write( "> makeFunction(\n"..tab..constValue.."Class& object, Return ( Class::*memberPointer )( " )
	coca.repeatCountedWriteSkipLast( file, level, "Argument", nil, ", " )
	file:write( " ) "..constValue..")\n{\n" )
	file:write( tab.."return Function<Return" )
	coca.repeatCountedWrite( file, level, ",Argument" )
	file:write( ">( createObjectFunctionAdaptor( object, memberPointer ) );\n}\n\n" )
	
	file:write( "template<typename FunctionReturn" )
	coca.repeatCountedWrite( file, level, ", typename FunctionArgument" )
	file:write( ", typename Return, typename Class" )
	coca.repeatCountedWrite( file, level, ", typename Argument" )
	file:write( ">\n" )
	file:write( "inline Function<FunctionReturn" )
	coca.repeatCountedWrite( file, level, ",FunctionArgument" )
	file:write( "> makeFunction(\n"..tab..constValue.."Class& object, Return ( Class::*memberPointer )( " )
	coca.repeatCountedWriteSkipLast( file, level, "Argument", nil, ", " )
	file:write( " ) "..constValue..")\n{\n" )
	file:write( tab.."return Function<FunctionReturn" )
	coca.repeatCountedWrite( file, level, ",FunctionArgument" )
	file:write( ">( createObjectFunctionAdaptor<FunctionReturn" )
	coca.repeatCountedWrite( file, level, ",FunctionArgument" )
	file:write( ">( object, memberPointer ) );\n}\n\n" )
end

-- function header --
function coca.generateFunctionHeaderFile( maxLevel, destination, namespace )
	print( "Generating function header file" )

	local filename = destination.."/function.h"
	local file, err = io.open( filename, "w" )
	assert( file, err )
	local guard = "AGEN_FUNCTION_H_INCLUDED"
	if namespace ~= nil then guard = string.upper( namespace ).."_"..guard end

	-- header
	coca.writeHeader( file, guard, nil, nil )

	-- all headers
	local level = 0
	while level <= maxLevel do
		file:write( "\n" )
		file:write( "#include \"RefMemberFunction"..level..".h\"\n" )
		file:write( "#include \"PtrMemberFunction"..level..".h\"\n" )
		file:write( "#include \"ObjectFunction"..level..".h\"\n" )
		file:write( "#include \"IFunctionAdaptor"..level..".h\"\n" )
		file:write( "#include \"ObjectFunctionAdaptor"..level..".h\"\n" )
		file:write( "#include \"Function"..level..".h\"\n" )
		level = level+1
	end

	-- footer
	coca.writeFooter( file, guard, nil )

	io.close( file )
end
