
#ifndef CORE_FILENAME_HPP
#define CORE_FILENAME_HPP

#include "string.hpp"

namespace core
{
	// filename manipulation functions

	String GetFilenameEXT(const String& filename);		// get extension of given filename
	String GetFilenamePATH(const String& filename);		// get path of given filename
	String GetFilenameNOEXT(const String& filename);	// get filename without extension
	String GetFilenameNOPATH(const String& filename);	// get filename without path

} // namespace core


#endif