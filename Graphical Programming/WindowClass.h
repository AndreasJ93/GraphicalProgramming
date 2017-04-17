#ifndef WINDOW_CLASS_H
#define WINDOW_CLASS_H

#include <Windows.h>
#include <stdlib.h>
#include <string.h>
#include <tchar.h>
#include "InputControl.h"

/*Make Singleton?*/
class WindowClass
{
private:
	//Set the "normal" values to things we need to keep track on.
	LPCWSTR _applicationName = L"Some text";
	UINT16 _width = 1280;
	UINT16 _height = 720;
	bool _fullScreen = false;

	//These values are given by the caller.
	HINSTANCE _hInstance;
	HWND _handle;
	
public:
	WindowClass();
	~WindowClass();

	HWND GetWindowHandle() const;
	UINT16 GetWindowWidth() const;
	UINT16 GetWindowHeight() const;
	bool GetWindowFullScreen() const;


	void SetTextForWindow(const LPCWSTR &newText);
	void WriteToWindow();

	bool Initialize(UINT16 width = 1280, UINT16 height = 720, const bool full_screen = false);
	void Shutdown();

	LRESULT CALLBACK MessageHandler(HWND hwnd, UINT umsg, WPARAM wParam, LPARAM lParam);

};

//WndProc and ApplicationHandle is used to make sure we can handle windows messages
static LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
static WindowClass* ApplicationHandle = 0;






#endif