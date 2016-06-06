
#include <core/filename.hpp>
#include <cstring>

using namespace core;


String core::GetFilenameEXT(const String& filename)
{
	int length = filename.GetLength();
	const char* text = filename + length;

	while ( length-- )
	{
		if ( *--text == '.' )
			return String(text + 1);
	}

	return String();
}


String core::GetFilenamePATH(const String& filename)
{
	int length = filename.GetLength();
	const char* text = filename + length;

	while ( length-- )
	{
		char v = *--text;
		if ( v == '\\' || v == '/' || v == ':' )
		{
			int size = text - static_cast<const char*>(filename) + 1;
			return filename.SubString(0,size);
		}
	}

	return String();
}


String core::GetFilenameNOEXT(const String& filename)
{
	int length = filename.GetLength();
	const char* text = filename + length;

	while ( length-- )
	{
		if ( *--text == '.' )
		{
			int size = text - static_cast<const char*>(filename);
			return filename.SubString(0,size);
		}
	}

	return filename;
}


String core::GetFilenameNOPATH(const String& filename)
{
	int length = filename.GetLength();
	const char* text = filename + length;

	while ( length-- )
	{
		char v = *--text;
		if ( v == '\\' || v == '/' || v == ':' )
			return String(text + 1);
	}

	return filename;
}
