#include "NativeWindow.h"

NativeWindow::NativeWindow()
{
}


NativeWindow::~NativeWindow()
{
}

bool NativeWindow::Create( HINSTANCE hInstance, int nCmdShow )
{
	// Regiser window class
	Register();

	// Create window
	int x = GetSystemMetrics(SM_CXFULLSCREEN) / 4;
	int y = 100;
	hWnd = CreateWindowEx( 0, APPCLASS, APPCLASS, WS_OVERLAPPEDWINDOW | WS_VISIBLE, x, y, iWidth, iHeight, nullptr, nullptr, hInstance, nullptr );

	if (!hWnd)
		return false;

	UpdateWindow( hWnd );
	ShowWindow( hWnd, nCmdShow );

	return true;
}

void NativeWindow::Register()
{
	WNDCLASSEX wcx{ 0 };
	wcx.cbSize = sizeof( wcx );
	wcx.style = CS_HREDRAW | CS_VREDRAW;
	wcx.hInstance = hInstance;
	wcx.hCursor = LoadCursor( nullptr, IDC_ARROW );
	wcx.hIcon = LoadIcon( nullptr, IDI_APPLICATION );
	wcx.hIconSm = wcx.hIcon;
	wcx.lpfnWndProc = NativeWindow::WndProc;
	wcx.lpszClassName = APPCLASS;
	
	RegisterClassEx( &wcx );
}

LRESULT CALLBACK NativeWindow::WndProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	switch (uMsg)
	{
		case WM_QUIT:
		case WM_DESTROY:
			PostQuitMessage( 0 );
			return TRUE;
	}
	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}
