
function coca.repeatWrite( file, count, text )
	local i = 1
	while i <= count do
		file:write( text )
		i = i+1
	end
end

function coca.repeatCountedWrite( file, count, text1, text2, postText )
	local i = 1
	while i <= count do
		if text1 ~= nil then file:write( text1..i ) end
		if text2 ~= nil then file:write( text2..i ) end
		if postText ~= nil then file:write( postText ) end
		i = i+1
	end
end

function coca.repeatCountedWriteSkipLast( file, count, text1, text2, postText )
	local i = 1
	while i <= count do
		if text1 ~= nil then file:write( text1..i ) end
		if text2 ~= nil then file:write( text2..i ) end
		if postText ~= nil and i < count then file:write( postText ) end
		i = i+1
	end
end
