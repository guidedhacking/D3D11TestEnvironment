#include "NativeWindow.h"
#include <stdio.h>
#include <d3d11.h>
#include <d2d1.h>
#include <dwrite.h>
#include <DirectXMath.h>
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d2d1.lib")
#pragma comment(lib, "dwrite.lib")

using namespace DirectX;

ID3D11Device* pDevice = nullptr;
IDXGISwapChain* pSwapchain = nullptr;
ID3D11DeviceContext* pDeviceContext = nullptr;
ID3D11RenderTargetView* pRenderTargetView = nullptr;

wchar_t szDevice[256]{ 0 };

// text rendering shenanigans
IDXGIDevice* pDXGIDevice = nullptr;
IDXGIFactory* pDXGIFactory = nullptr;
ID2D1Factory* pD2DFactory = nullptr;
IDXGIAdapter* pDXGIAdapter = nullptr;
IDXGISwapChain* pDXGISwapchain = nullptr;
IDXGISurface* pDXGISurface = nullptr;
ID2D1RenderTarget* pTextRenderTarget = nullptr;
ID2D1SolidColorBrush* pSolidBrush = nullptr;
IDWriteFactory* pDWriteFactory = nullptr;
IDWriteTextLayout* pTextLayout = nullptr;
IDWriteTextFormat* pTextFormat = nullptr;

bool InitD3D( HWND hWnd)
{
	HRESULT hr = S_OK;
	RECT rc{ 0 };

	GetClientRect( hWnd, &rc );
	UINT uWidth = rc.right - rc.left;
	UINT uHeight = rc.bottom - rc.top;

	UINT createDeviceFlags = D3D10_CREATE_DEVICE_BGRA_SUPPORT;
#ifdef _DEBUG
	createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

	DXGI_SWAP_CHAIN_DESC sd{ 0 };
	sd.BufferCount = 1;
	sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	sd.BufferDesc.Height = uHeight;
	sd.BufferDesc.Width = uWidth;
	sd.BufferDesc.RefreshRate = { 60, 1 };
	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	sd.OutputWindow = hWnd;
	sd.Windowed = TRUE;
	sd.SampleDesc.Count = 1;
	sd.SampleDesc.Quality = 0;

	D3D_FEATURE_LEVEL featureLevel;

	hr = D3D11CreateDeviceAndSwapChain( nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, createDeviceFlags,
										nullptr, 0, D3D11_SDK_VERSION, &sd, &pSwapchain, &pDevice, &featureLevel, &pDeviceContext );
	if (FAILED( hr ))
		return false;


	// Get a pointer to the back buffer for the render target view
	ID3D11Texture2D* pBackbuffer = nullptr;
	hr = pSwapchain->GetBuffer( 0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(&pBackbuffer) );
	if (FAILED( hr ))
		return false;

	// Create render target view
	hr = pDevice->CreateRenderTargetView( pBackbuffer, nullptr, &pRenderTargetView );
	pBackbuffer->Release();
	if (FAILED( hr ))
		return false;


	// text rendering shenanigans (directwrite)
	hr = pSwapchain->QueryInterface( __uuidof(IDXGISwapChain), reinterpret_cast<void**>(&pDXGISwapchain) );
	if (FAILED( hr ))
		return false;

	hr = pDXGISwapchain->GetBuffer( 0, __uuidof(IDXGISurface), reinterpret_cast<void**>(&pDXGISurface) );
	if (FAILED( hr ))
		return false;

	hr = D2D1CreateFactory( D2D1_FACTORY_TYPE_SINGLE_THREADED, &pD2DFactory );
	if (FAILED( hr ))
		return false;

	float dpiX, dpiY;
	pD2DFactory->GetDesktopDpi( &dpiX, &dpiY );
	D2D1_RENDER_TARGET_PROPERTIES rtp;
	ZeroMemory( &rtp, sizeof( rtp ) );
	rtp.dpiX = dpiX;
	rtp.dpiY = dpiY;
	rtp.type = D2D1_RENDER_TARGET_TYPE_DEFAULT;
	rtp.pixelFormat = D2D1::PixelFormat( DXGI_FORMAT_UNKNOWN, D2D1_ALPHA_MODE_PREMULTIPLIED );
	rtp.usage = D2D1_RENDER_TARGET_USAGE_NONE;

	hr = pD2DFactory->CreateDxgiSurfaceRenderTarget( pDXGISurface, &rtp, &pTextRenderTarget );
	if (FAILED( hr ))
		return false;

	hr = DWriteCreateFactory( DWRITE_FACTORY_TYPE_ISOLATED, __uuidof(IDWriteFactory), reinterpret_cast<IUnknown**>(&pDWriteFactory) );
	if (FAILED( hr ))
		return false;

	swprintf_s( szDevice, 256, L"DevicePtr: 0x%p\nSwapchainPtr: 0x%p\n", &pDevice, &pSwapchain );

	hr = pDWriteFactory->CreateTextFormat( L"Consolas", nullptr, DWRITE_FONT_WEIGHT_NORMAL, DWRITE_FONT_STYLE_NORMAL, DWRITE_FONT_STRETCH_NORMAL, 18.0f, L"en-us", &pTextFormat );
	if (FAILED( hr ))
		return false;

	hr = pDWriteFactory->CreateTextLayout( szDevice, lstrlenW( szDevice ), pTextFormat, uWidth, uHeight, &pTextLayout );
	if (FAILED( hr ))
		return false;

	D2D1_COLOR_F color = { 1.0f, 1.0f, 1.0f, 1.0f };
	hr = pTextRenderTarget->CreateSolidColorBrush( color, &pSolidBrush );
	if (FAILED( hr ))
		return false;

	return true;
}

float bgColor[4] = { 0.1f , 0.1f, 0.1f, 1.0f };
D2D1_POINT_2F textPos = { 0,0 };
void Render( NativeWindow& wnd )
{
	pDeviceContext->ClearRenderTargetView( pRenderTargetView, bgColor );
	pTextRenderTarget->BeginDraw();
	pTextRenderTarget->DrawTextLayout( textPos, pTextLayout, pSolidBrush );
	pTextRenderTarget->EndDraw();
	pSwapchain->Present( 0, 0 );
}

int WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow )
{
	NativeWindow wnd;

	wnd.Create( hInstance, nCmdShow );

	if (!InitD3D( wnd.GetHandle() ))
		return 1;

	MSG m;
	while (true)
	{
		while (PeekMessage( &m, NULL, 0, 0, PM_REMOVE ) && m.message != WM_QUIT)
		{
			TranslateMessage( &m );
			DispatchMessage( &m );
		}
		if (m.message == WM_QUIT)
			break;

		Render( wnd );
	}

	return 0;
}