#pragma once
#include <Windows.h>
#include <tchar.h>

#define APPCLASS _T( "D3D11Test" )
#define WND_WIDTH 800
#define WND_HEIGHT 600
class NativeWindow
{
	HINSTANCE hInstance = nullptr;
	HWND hWnd = nullptr;
	int iWidth = WND_WIDTH;
	int iHeight = WND_HEIGHT;
public:
	NativeWindow();
	~NativeWindow();

	bool Create( HINSTANCE hInstance, int nCmdShow );
	HWND GetHandle() { return hWnd; }

private:
	void Register();
	static LRESULT CALLBACK WndProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

};

