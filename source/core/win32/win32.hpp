
	#ifndef CORE_PLATFORM_WIN32
	#error "NEVER manually include this header."
	#endif

	#define CORE_CONFIGURED

	#ifdef _DEBUG
	#define CORE_DEBUG
	#endif

	// don't define min/max macros

	#ifndef NOMINMAX
	#define NOMINMAX
	#endif

	// win32 includes

	//#define WIN32_LEAN_AND_MEAN
	#define _WIN32_WINNT 0x0500 // kludge to enable mouse wheel in win32 api
	#include <windows.h>
	#include <process.h>

	// floating-point types

	typedef float				float32;
	typedef double				float64;

	// integer types

	typedef	signed char			int8;
	typedef	unsigned char		uint8;
	typedef	signed short		int16;
	typedef	unsigned short		uint16;
	typedef signed int			int32;
	typedef unsigned int		uint32;
	typedef signed __int64		int64;
	typedef unsigned __int64	uint64;

	// unicode chartype

	typedef wchar_t				uchar16;

	// OS types

	struct WindowHandle
	{
		HWND		hwnd;
		HINSTANCE	hinstance;	
		HDC			hdc;
	};

	struct ContextHandle
	{
		HDC			hdc;
		HGLRC		hrc;
		HWND		hwnd;
	};

	typedef HANDLE ThreadHandle;
	typedef CRITICAL_SECTION MutexHandle;







