
#include <core/window.hpp>
#include <core/timer.hpp>

using namespace core;


static LRESULT CALLBACK WndProc(HWND hwnd, UINT imsg, WPARAM wparam, LPARAM lparam)
{
	// get window pointer
	Window* window = reinterpret_cast<Window*>(GetWindowLong(hwnd,GWL_USERDATA));

	switch ( imsg )
	{
		//case WM_CREATE:
			//break;

		case WM_TIMER:
			break;

		case WM_PAINT:
		{
			PAINTSTRUCT paint;
			BeginPaint(hwnd,&paint);

			if ( window && window->GetEnabled() )
				window->EventDraw();

			EndPaint(hwnd,&paint);
			ValidateRect(hwnd,NULL);

			return 0;
		}

		case WM_SIZE:
		{
			if ( window && window->GetEnabled() )
			{
				int width  = LOWORD(lparam);
				int height = HIWORD(lparam);
				window->EventSize(width,height);
			}
			return 0;
		}

		case WM_KEYDOWN:
		case WM_KEYUP:
		{
			return 0;
		}

		case WM_MOUSEMOVE:
		{
			return 0;
		}

		case WM_ACTIVATE:
		{
			if (!HIWORD(wparam))
			{
				window->SetEnabled(true);
			}
			else
			{
				window->SetEnabled(false);
			}

			if ( window && window->GetEnabled() )
			{
				bool enter = LOWORD(wparam) != WA_INACTIVE;
				window->EventFocus(enter);
			}
			return 0;
		}
		
		case WM_DROPFILES:
		{
			char filename[512];
			if ( DragQueryFile((HDROP)wparam,0,filename,sizeof(filename)) > 0 )
			{
				if ( window && window->GetEnabled() )
					window->EventDropFile(filename);
			}
			return 0;
		}

		case WM_DESTROY:
		{
			SetWindowLong(hwnd,GWL_USERDATA,(LONG)NULL);
			return 0;
		}

		case WM_CLOSE:
		{
			PostQuitMessage(0);
			return 0;
		}

		case WM_SYSCOMMAND:
		{
			switch (wparam)
			{
				case SC_SCREENSAVE:
				case SC_MONITORPOWER:
				return 0;
			}
			break;
		}
	}

	return DefWindowProc(hwnd,imsg,wparam,lparam);
}

Window::Window() :
	opened(false),
	active(false),
	quit(false)
{
}

Window::~Window()
{
	Close();
}

static char* WndClassName = "wndamphora";

bool Window::Open(int width, int height, const char* name, bool fullscreen)
{
	if(opened) return true;
	handle.hinstance = GetModuleHandle(NULL);

	RECT WindowRect;
	WindowRect.left = (long)0;
	WindowRect.right = (long)width;
	WindowRect.top = (long)0;
	WindowRect.bottom = (long)height;

	WNDCLASS wc;
	wc.style			= CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	wc.lpfnWndProc		= (WNDPROC) WndProc;
	wc.cbClsExtra		= 0;
	wc.cbWndExtra		= 0;
	wc.hInstance		= handle.hinstance;
	wc.hIcon			= LoadIcon(NULL, IDI_WINLOGO);
	wc.hCursor			= LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground	= NULL;
	wc.lpszMenuName		= NULL;
	wc.lpszClassName	= WndClassName;	

	if (!RegisterClass(&wc))
		CORE_THROW("Failed to register the window class");
	
	DWORD dwExStyle = WS_EX_APPWINDOW;
	DWORD dwStyle = WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX;

	AdjustWindowRectEx(&WindowRect, dwStyle, FALSE, dwExStyle);

	if (!(handle.hwnd=CreateWindowEx(dwExStyle,							// Extended Style For The Window
								WndClassName,							// Class Name
								name,								// Window Title
								dwStyle |							// Defined Window Style
								WS_CLIPSIBLINGS |					// Required Window Style
								WS_CLIPCHILDREN,					// Required Window Style
								0, 0,								// Window Position
								WindowRect.right-WindowRect.left,	// Calculate Window Width
								WindowRect.bottom-WindowRect.top,	// Calculate Window Height
								NULL,								// No Parent Window
								NULL,								// No Menu
								NULL,							// Instance
								NULL)))								// Dont Pass Anything To WM_CREATE
		CORE_THROW("Failed to create window");

	// set win32 hwnd userdata to point to the window object
	SetWindowLong(handle.hwnd,GWL_USERDATA,(LONG)this);
	UpdateWindow(handle.hwnd);

	DragAcceptFiles(handle.hwnd,TRUE);

	opened = true;
	this->width = width;
	this->height = height;

	ShowWindow(handle.hwnd,SW_SHOW);
	SetForegroundWindow(handle.hwnd);
	SetFocus(handle.hwnd);

	return true;
}

void Window::Close()
{
	if(!opened) return;

	if(handle.hdc && !ReleaseDC(handle.hwnd,handle.hdc))
		handle.hdc=NULL;

	if(handle.hwnd && !DestroyWindow(handle.hwnd))
		handle.hwnd=NULL;

	if(!UnregisterClass(WndClassName,NULL))
		handle.hinstance=NULL;
}


void Window::Run()
{
	//	Force the main thread to always run on CPU 0.
	//	This is done because on some systems QueryPerformanceCounter returns a bit different counter values
	//	on the different CPUs (contrary to what it's supposed to do), which can cause negative frame times
	//	if the thread is scheduled on the other CPU in the next frame. This can cause very jerky behavior and
	//	appear as if frames return out of order.
	SetThreadAffinityMask(GetCurrentThread(), 1);

	// update window before we enter the mainloop
	// .. so that the EventDraw() is called atleast once!
	UpdateWindow(handle.hwnd);

	// enter the message loop
	MSG msg;
	ZeroMemory(&msg,sizeof(msg));

	// mainbreak:false
	quit = false;

	Timer timer;
	float oldtime = timer.GetTime();

	// message loop
	while ( msg.message != WM_QUIT && quit == false )
	{
		if ( ::PeekMessage(&msg,NULL,0U,0U,PM_REMOVE) )
		{
			::TranslateMessage(&msg);
			::DispatchMessage(&msg);
		}
		else
		{
			float time = timer.GetTime();
			float dt = time-oldtime;
			oldtime = time;

			Update(dt);
		}
	}

}


void Window::Rename(const char* name)
{
	const char* text = name;
	SetWindowText(handle.hwnd,text);
}

void AdjustSize(uint32 mask, int& width, int& height)
{
	RECT r;

	r.left 	 = 0;
	r.top 	 = 0;
	r.right  = width - 1;
	r.bottom = height - 1;

	AdjustWindowRect(&r,mask,FALSE);

	width  = r.right - r.left + 1;
	height = r.bottom - r.top + 1;
}

void Window::Resize(int width, int height)
{
	LONG mask = GetWindowLong(handle.hwnd,GWL_STYLE);
	AdjustSize(mask,width,height);

	SetWindowPos(handle.hwnd,HWND_TOP,0,0,width,height,SWP_NOMOVE|SWP_NOZORDER);
	this->width = width;
	this->height = height;
}

void Window::Move(int x, int y)
{
	SetWindowPos(handle.hwnd,HWND_TOP,x,y,0,0,SWP_NOSIZE|SWP_NOZORDER);
}

void Window::SetCursor(bool value)
{
	if(value)
	{
		while ( ShowCursor(TRUE) < 0 )
		{
		}
	}
	else
	{
		while ( ShowCursor(FALSE) >= 0 )
		{
		}

		// cursor won't be hidden until it gets a move message
		SetCursorPos(0,0);
		SetCursorPos(1,0);
	}
}

void Window::SetEnabled(bool value)
{
	active = value;
}

bool Window::GetEnabled() const
{
	return active;
}

void Window::Quit()
{
	quit = true;
}

