
#include <core/common.hpp>

#include <string.h>
#include <stdio.h>
#include <stdarg.h>

using namespace core;


void core::Printf(const char* fmt, ...)
{
	// format variable arguments
	const unsigned MAX_MSG = 2000;
	char msg[MAX_MSG+4];
	va_list marker;
	va_start( marker, fmt );
	vsprintf( msg, fmt, marker );
	va_end( marker );
	CORE_ASSERT( strlen(msg) < MAX_MSG ); // too long debug message

	OutputDebugStringA(msg);
	//printf(msg);
}

