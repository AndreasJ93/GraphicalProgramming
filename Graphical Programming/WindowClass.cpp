#include "WindowClass.h"

#include "InputControl.h"

WindowClass::WindowClass()
{
	ApplicationHandle = this;
}

WindowClass::~WindowClass()
{
}

HWND WindowClass::GetWindowHandle() const
{
	return _handle;
}

UINT16 WindowClass::GetWindowWidth() const
{
	return _width;
}

UINT16 WindowClass::GetWindowHeight() const
{
	return _height;
}

bool WindowClass::GetWindowFullScreen() const
{
	return _fullScreen;
}

void WindowClass::SetTextForWindow(const LPCWSTR &newText)
{
	SetWindowText(_handle, newText);
}

void WindowClass::WriteToWindow()
{
	PAINTSTRUCT ps;
	HDC hDC = BeginPaint(_handle, &ps);
	
	TextOut(hDC, 200, 200, L"Testing\nThis!", 13);
	
	EndPaint(_handle, &ps);
}

bool WindowClass::Initialize(UINT16 width, UINT16 height, bool full_screen)
{
	WNDCLASSEX wc;
	DEVMODE dmScreenSettings;
	int posX, posY;
	_width = width;
	_height = height;
	_fullScreen = full_screen;

	//Get the instance of this application
	_hInstance = GetModuleHandle(NULL);

	//Set the name of the application
	_applicationName = L"3D Basic Project";

	//Setup the windows class with default settings
	wc.style = 0;
	wc.lpfnWndProc = WndProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = _hInstance;
	wc.hIcon = LoadIcon(NULL, IDI_WINLOGO);
	wc.hIconSm = wc.hIcon;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	wc.lpszMenuName = NULL;
	wc.lpszClassName = _applicationName;
	wc.cbSize = sizeof(WNDCLASSEX);

	//Register the window class
	if (!RegisterClassEx(&wc))
	{
		MessageBox(NULL, L"Window Registration Failed!", L"Error!",
			MB_ICONEXCLAMATION | MB_OK);
		return 0;
	}

	//Setup the screen settings. If it's in fullscreen, we specify for that.
	if (full_screen)
	{
		// If full screen set the screen to maximum size of the users desktop and 32bit.
		memset(&dmScreenSettings, 0, sizeof(dmScreenSettings));
		dmScreenSettings.dmSize = sizeof(dmScreenSettings);
		dmScreenSettings.dmPelsWidth = width;
		dmScreenSettings.dmPelsHeight = (unsigned long)height;
		dmScreenSettings.dmBitsPerPel = 64;
		dmScreenSettings.dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;

		// Change the display settings to full screen.
		ChangeDisplaySettings(&dmScreenSettings, CDS_FULLSCREEN);

		// Set the position of the window to the top left corner.
		posX = posY = 0;

		// Create the window with the screen settings and get the handle to it.
		_handle = CreateWindowEx(WS_EX_APPWINDOW, _applicationName, _applicationName,
			WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_POPUP,
			posX, posY, width, height, NULL, NULL, _hInstance, NULL);

		//Hide the cursor
		ShowCursor(false);

	}
	else
	{

		// Place the window in the middle of the screen.
		posX = (GetSystemMetrics(SM_CXSCREEN) - width) / 2;
		posY = (GetSystemMetrics(SM_CYSCREEN) - height) / 2;

		// Create the window with the screen settings and get the handle to it.
		_handle = CreateWindowEx(WS_EX_APPWINDOW, _applicationName, _applicationName,
			WS_CAPTION | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_POPUP | WS_SYSMENU,
			posX, posY, width, height, NULL, NULL, _hInstance, NULL);
	}

	if (_handle == NULL)
	{
		MessageBox(NULL, L"Window Creation Failed!", L"Error!",
			MB_ICONEXCLAMATION | MB_OK);
		return 0;
	}

	// Bring the window up on the screen and set it as main focus.
	ShowWindow(_handle, SW_SHOW);
	
	SetFocus(_handle);


	return true;
}

void WindowClass::Shutdown()
{
	//Fix the display settings if we leave full screen
	if (_fullScreen)
	{
		ShowCursor(true);
		ChangeDisplaySettings(NULL, 0);
	}

	//Remove the window
	DestroyWindow(_handle);
	_handle = NULL;

	//Remove the application instance
	UnregisterClass(_applicationName, _hInstance);
	_hInstance = NULL;

	//Release the pointer to the class.
	ApplicationHandle = NULL;
}

LRESULT CALLBACK WindowClass::MessageHandler(HWND hwnd, UINT umsg, WPARAM wParam, LPARAM lParam)
{
	switch (umsg)
	{
		//Key is pressed
	case WM_KEYDOWN:
		InputControl_Singleton::Instance().KeyDown((unsigned int) wParam);
		break;
		
		//Key is released
	case WM_KEYUP:
		InputControl_Singleton::Instance().KeyUp((unsigned int) wParam);
		break;

		/*Input other checks here*/



	}

	return  DefWindowProc(hwnd, umsg, wParam, lParam);
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT umsg, WPARAM wParam, LPARAM lParam)
{
	//Ugly "escape to exit" solution
	if ((unsigned long)wParam == VK_ESCAPE)
	{
		PostQuitMessage(0);
		return 0;
	}

	switch (umsg)
	{
		// Check if the window is being destroyed.
	case WM_DESTROY:
	{
		PostQuitMessage(0);
		break;
	}

	// Check if the window is being closed.
	case WM_CLOSE:
	{
		PostQuitMessage(0);
		break;
	}

	// All other messages pass to the message handler in the system class.
	default:
	{
		return ApplicationHandle->MessageHandler(hwnd, umsg, wParam, lParam);
	}
	}

	return 0;
}
