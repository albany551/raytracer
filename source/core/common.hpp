
#ifndef CORE_UTILS_HPP
#define CORE_UTILS_HPP

#include <core/exception.hpp>

// =============================================
// common functions
// =============================================

namespace core
{

	void	Printf(const char* fmt, ...);

/*
	String	GetFilenameEXT(const String& filename);		// get extension of given filename
	String	GetFilenamePATH(const String& filename);	// get path of given filename
	String	GetFilenameNOEXT(const String& filename);	// get filename without extension
	String	GetFilenameNOPATH(const String& filename);	// get filename without path
*/

};

// =============================================
// common definitions
// =============================================

#define CORE_NEW blah
#define CORE_DLETE blah

#ifndef CORE_FILENAME
#ifndef __FILE__
#define CORE_FILENAME		"Unknown File"
#else
#define CORE_FILENAME		__FILE__
#endif
#endif

#ifndef CORE_FUNCTION
#ifndef __FUNCTION__
#define CORE_FUNCTION		"Unknown function"
#else
#define CORE_FUNCTION		__FUNCTION__
#endif
#endif

#ifndef CORE_LINE
#ifndef __LINE__
#define CORE_LINE			-1
#else
#define CORE_LINE			__LINE__
#endif
#endif

// =============================================
// platform specific configuration
// =============================================

#ifndef CORE_CONFIGURED
#if defined(_WIN32) || defined(_WINDOWS_)

	#define CORE_PLATFORM_WIN32
	#define CORE_LITTLE_ENDIAN

	#include "win32/win32.hpp"

#endif
#endif

#ifndef CORE_CONFIGURED
#if defined(__linux__)

	#define CORE_PLATFORM_LINUX
	#define CORE_LITTLE_ENDIAN

	#include "linux/linux.hpp"

#endif
#endif


#ifndef CORE_CONFIGURED
#error "platform not supported"
#endif



#endif
