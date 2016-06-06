
#include <core/mouse.hpp>
#include <core/common.hpp>

using namespace core;


// =================================================
// statics
// =================================================

static LRESULT CALLBACK mouseStaticHook(int ncode, WPARAM wparam, LPARAM lparam);

static int mouseCounts[6] = { 0,0,0,0,0,0 };
static int mouseX = 0;
static int mouseY = 0;
static int lastX = -1;
static int lastY = -1;
static HHOOK mouseStaticHookId = 0;
static long mouseStaticInitCount = 0;


// =================================================
// compatibility
// =================================================

#ifndef WM_XBUTTONUP
#define WM_XBUTTONUP		0x020C
#endif

#ifndef WM_XBUTTONDOWN
#define WM_XBUTTONDOWN		0x020B
#endif

#ifndef WM_XBUTTONDBLCLK
#define WM_XBUTTONDBLCLK	0x020D
#endif

#ifndef XBUTTON1
#define XBUTTON1			0x0001
#endif

#ifndef XBUTTON2
#define XBUTTON2			0x0002
#endif

#ifndef GET_XBUTTON_WPARAM
#define GET_XBUTTON_WPARAM(w)	(HIWORD(w))
#endif

#ifndef GET_WHEEL_DELTA_WPARAM
#define GET_WHEEL_DELTA_WPARAM(wParam) ((short)HIWORD(wParam))

typedef struct tagMOUSEHOOKSTRUCTEX {
	POINT     pt; 
	HWND      hwnd; 
	UINT      wHitTestCode; 
	DWORD     dwExtraInfo; 
	DWORD     mouseData;
} MOUSEHOOKSTRUCTEX;
#endif


Mouse::Mouse()
{
	mouseStaticInitCount++;

	if ( ( !mouseStaticHookId ) && ( mouseStaticInitCount <= 1 ) )
	{
		mouseStaticHookId = SetWindowsHookEx( WH_MOUSE,
			(HOOKPROC) mouseStaticHook, 0, GetCurrentThreadId() );

		if ( !mouseStaticHookId ) 
			return;
	}
}


Mouse::~Mouse()
{
	if ( mouseStaticInitCount > 0 ) mouseStaticInitCount--;

	if ( ( mouseStaticHookId ) && ( mouseStaticInitCount == 0 ) )
	{
		UnhookWindowsHookEx( mouseStaticHookId );
		mouseStaticHookId = 0;

		ClipCursor( NULL );	// restore when done
	}
}


void Mouse::SetXY(int x, int y)
{
	SetCursorPos( x, y );
}


int Mouse::GetX()
{
	return mouseX;
}


int Mouse::GetY()
{
	return mouseY;
}

int Mouse::GetDeltaX()
{
	int delta = mouseX - lastX;
	lastX = mouseX;
	return delta;
}


int Mouse::GetDeltaY()
{
	int delta = mouseY - lastY;
	lastY = mouseY;
	return delta;
}

int Mouse::GetButton(int index)
{
	switch ( index )
	{
		case MOUSE_LEFT:	return mouseCounts[MOUSE_LEFT];
		case MOUSE_RIGHT:	return mouseCounts[MOUSE_RIGHT];
		case MOUSE_MIDDLE:	return mouseCounts[MOUSE_MIDDLE];
		case MOUSE_XBUTTON1:return mouseCounts[MOUSE_XBUTTON1];
		case MOUSE_XBUTTON2:return mouseCounts[MOUSE_XBUTTON2];
		case MOUSE_WHEEL:	return mouseCounts[MOUSE_WHEEL];
		default:			return 0;
	}
}


static LRESULT CALLBACK mouseStaticHook(int ncode, WPARAM wparam, LPARAM lparam)
{
	if ( ncode < 0 )
	{
		return (int)CallNextHookEx(mouseStaticHookId,ncode,wparam,lparam);
	}

	MOUSEHOOKSTRUCTEX& mhs = *(MOUSEHOOKSTRUCTEX*)lparam;
	mouseX = mhs.pt.x;
	mouseY = mhs.pt.y;

	switch ( wparam )
	{
		case WM_LBUTTONDOWN:
			mouseCounts[Mouse::MOUSE_LEFT] = 1;
			break;
		case WM_LBUTTONUP:
			mouseCounts[Mouse::MOUSE_LEFT] = 0;
			break;
		case WM_LBUTTONDBLCLK:
			mouseCounts[Mouse::MOUSE_LEFT] = 2;
			break;


		case WM_MBUTTONDOWN:
			mouseCounts[Mouse::MOUSE_MIDDLE] = 1;
			break;
		case WM_MBUTTONUP:
			mouseCounts[Mouse::MOUSE_MIDDLE] = 0;
			break;
		case WM_MBUTTONDBLCLK:
			mouseCounts[Mouse::MOUSE_MIDDLE] = 2;
			break;

		case WM_RBUTTONDOWN:
			mouseCounts[Mouse::MOUSE_RIGHT] = 1;
			break;
		case WM_RBUTTONUP:
			mouseCounts[Mouse::MOUSE_RIGHT] = 0;
			break;
		case WM_RBUTTONDBLCLK:
			mouseCounts[Mouse::MOUSE_RIGHT] = 2;
			break;

		case WM_XBUTTONDOWN:
			switch ( GET_XBUTTON_WPARAM(mhs.mouseData) )
			{
				case XBUTTON1: mouseCounts[Mouse::MOUSE_XBUTTON1] = 1; break;
				case XBUTTON2: mouseCounts[Mouse::MOUSE_XBUTTON2] = 1; break;
				default: break;
			}
			break;
		case WM_XBUTTONUP:
			switch ( GET_XBUTTON_WPARAM(mhs.mouseData) )
			{
				case XBUTTON1: mouseCounts[Mouse::MOUSE_XBUTTON1] = 0; break;
				case XBUTTON2: mouseCounts[Mouse::MOUSE_XBUTTON2] = 0; break;
				default: break;
			}
			break;
		case WM_XBUTTONDBLCLK:
			switch ( GET_XBUTTON_WPARAM(mhs.mouseData) )
			{
				case XBUTTON1: mouseCounts[Mouse::MOUSE_XBUTTON1] = 2; break;
				case XBUTTON2: mouseCounts[Mouse::MOUSE_XBUTTON2] = 2; break;
				default: break;
			}
			break;

		case WM_MOUSEWHEEL:
			mouseCounts[Mouse::MOUSE_WHEEL] += GET_WHEEL_DELTA_WPARAM(mhs.mouseData);
			break;

		default:
			break;
	}

	return CallNextHookEx(mouseStaticHookId,ncode,wparam,lparam);
}
