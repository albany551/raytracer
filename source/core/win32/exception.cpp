
#include <core/exception.hpp>

#include <core/common.hpp>
#include <cstdlib>
#include <cstring>

using namespace core;

static long		eExceptionWindowWidth;
static long		eExceptionWindowHeight;

static char	eExceptionLinenr[16];
static const char*	eExceptionMethod;
static const char*	eExceptionFilename;
static const char*	eExceptionMessage;

Exception::Exception(const char* method, const char* message, const char* filename, int line) : 
	filename(filename), method(method), message(message), line(line)
{
}

static void ExceptionTextCaption(HDC hdc, long x, long y, const char* s)
{
	if ( s )
	{
		HFONT o = (HFONT)SelectObject(hdc,(HFONT)GetStockObject(SYSTEM_FONT));
		TextOut(hdc,x,y,s,(int)strlen(s));
	 	SelectObject(hdc,o);
	}
}


static void ExceptionText(HDC hdc, long x, long y, const char* s)
{
	if ( s )
	{
		HFONT oldfont = (HFONT)SelectObject( hdc, (HFONT) GetStockObject( DEFAULT_GUI_FONT ) );
		TextOut(hdc,x,y,s,(int)strlen(s));
	 	SelectObject(hdc,oldfont);
	}
}


static LONG APIENTRY ExceptionWindow(HWND h, UINT m, UINT w, LONG l)
{
	switch ( m )
	{

		case WM_CREATE:
		{
			HDC hdc = GetDC(h);
			SetMapMode(hdc,MM_TEXT);

			SIZE size;
			int maxwidth = 0;
			HFONT oldfont = (HFONT)SelectObject( hdc, (HFONT) GetStockObject( DEFAULT_GUI_FONT ) );
			GetTextExtentPoint32( hdc,eExceptionMethod,(int)strlen(eExceptionMethod),&size );
			if (size.cx > maxwidth) maxwidth = size.cx;
			GetTextExtentPoint32( hdc,eExceptionFilename,(int)strlen(eExceptionFilename),&size );
			if (size.cx > maxwidth) maxwidth = size.cx;
			GetTextExtentPoint32( hdc,eExceptionMessage,(int)strlen(eExceptionMessage),&size );
			if (size.cx > maxwidth) maxwidth = size.cx;
		 	SelectObject(hdc,oldfont);

			ReleaseDC(h, hdc);

			RECT rect;
			GetClientRect(h, &rect);

			int delta = maxwidth + 208 - rect.right + rect.left;
			if ( delta > 0 )
			{
				if ( eExceptionWindowWidth + delta > GetSystemMetrics(SM_CXFULLSCREEN))
					delta = GetSystemMetrics(SM_CXFULLSCREEN) - eExceptionWindowWidth;
				eExceptionWindowWidth += delta;

				long ex = (GetSystemMetrics(SM_CXFULLSCREEN) / 2) - eExceptionWindowWidth / 2;
				long ey = (GetSystemMetrics(SM_CYFULLSCREEN) / 2) - eExceptionWindowHeight / 2;

				SetWindowPos(h, HWND_TOP, ex, ey, eExceptionWindowWidth, eExceptionWindowHeight, SWP_SHOWWINDOW);
			}
			else
			{
				delta = 0;
			}

			break;
		}

		case WM_COMMAND:
		{
			switch( LOWORD(w) )
			{
				case 120:
				{
					DestroyWindow(h);
					break;
				}
			}

			break;
		}

		case WM_ERASEBKGND:
		{
			RECT r;
			r.top    = 0;
			r.left   = 0;
			r.right  = eExceptionWindowWidth;
			r.bottom = eExceptionWindowHeight;

			HBRUSH b = CreateSolidBrush(RGB(255,255,255));
			HBRUSH o = (HBRUSH)SelectObject((HDC)w,b);

			FillRect((HDC)w,&r,b);

			SelectObject((HDC)w,o);
			DeleteObject(b);
			break;
		}


		case WM_PAINT:
		{
			PAINTSTRUCT ps;

			HDC hdc = BeginPaint(h,&ps);
			SetMapMode(hdc,MM_TEXT);
			SetBkColor(hdc,RGB(255,255,255));

			ExceptionTextCaption(hdc,10,8,"Software Exception");
			ExceptionText(hdc,10,25,"filename:");
			ExceptionText(hdc,10,40,"function:");
			ExceptionText(hdc,10,55,"line:");
			ExceptionText(hdc,10,70,"message:");
			ExceptionText(hdc,70,25,eExceptionFilename);
			ExceptionText(hdc,70,40,eExceptionMethod);
			ExceptionText(hdc,70,55,eExceptionLinenr);
			ExceptionText(hdc,70,70,eExceptionMessage);

			EndPaint(h,&ps);

			return 0;
		}

		case WM_INITMENU:
		{
			break;
		}

		case WM_KEYDOWN:
		{
			switch ( w )
			{
				case VK_ESCAPE:
				case VK_SPACE:
				case VK_RETURN:
					DestroyWindow(h);
					break;
			}
		}

		case WM_CLOSE:
		{
			DestroyWindow(h);
			break;
		}

		case WM_DESTROY:
		{
			PostQuitMessage(0);
			break;
		}

		default:
		{
			return (LONG)DefWindowProc(h,m,w,l);
		}
	}

	return 0;
}


// =================================================
// Exception::Display()
// =================================================

void Exception::Display()
{
	static const char* wndclass = "ExceptionWindowClass";

	eExceptionMethod	= method;
	eExceptionFilename	= filename;
	eExceptionMessage	= message;
	
	if ( line != -1 )
	{
		
		//eExceptionLinenr.PrintText("%d",mLinenr);
		_itoa(line,eExceptionLinenr,10);
	}
	else
	{
		strcpy(eExceptionLinenr, "N/A");
	}
	
	// main window
	WNDCLASS c;
    c.style = 0;
    c.lpfnWndProc = (WNDPROC)ExceptionWindow;
    c.cbClsExtra = 0;
    c.cbWndExtra = 0;
    c.hInstance = GetModuleHandle( NULL );
    c.hIcon = LoadIcon( NULL, IDI_EXCLAMATION );
    c.hCursor = LoadCursor(NULL, IDC_ARROW);
    c.hbrBackground = (HBRUSH) (COLOR_MENU + 1);
    c.lpszMenuName = NULL;
    c.lpszClassName = wndclass;
    RegisterClass(&c);

	// calculate the window dimensions, and show it
	RECT r;

	r.left 	 = 0;
	r.top 	 = 0;
	r.right  = 256;
	r.bottom = 94;

	eExceptionWindowWidth  = (r.right - r.left);
	eExceptionWindowHeight = (r.bottom - r.top);

	long ex = (GetSystemMetrics(SM_CXFULLSCREEN) / 2) - eExceptionWindowWidth / 2;
	long ey = (GetSystemMetrics(SM_CYFULLSCREEN) / 2) - eExceptionWindowHeight / 2;

	if ( !AdjustWindowRectEx( &r, WS_SYSMENU | WS_CLIPCHILDREN | WS_POPUP | WS_CAPTION | WS_BORDER, FALSE, WS_EX_TOPMOST ) ) 
		return;

	eExceptionWindowWidth  = (r.right - r.left);
	eExceptionWindowHeight = (r.bottom - r.top);

	// create dialog window
	HWND eHwnd = CreateWindowEx( WS_EX_TOPMOST, wndclass, "Error",
		WS_CAPTION | WS_POPUP | WS_BORDER | WS_CLIPCHILDREN | WS_SYSMENU,
		ex, ey, (r.right - r.left), (r.bottom - r.top), NULL, NULL, GetModuleHandle( NULL ), NULL );

	ShowWindow( eHwnd, SW_SHOW );
	UpdateWindow( eHwnd );

	// message loop
	MSG eMsg;
	while ( ::GetMessage(&eMsg,NULL,0,0) )
	{
		::TranslateMessage(&eMsg);
   		::DispatchMessage(&eMsg);
	}
}
