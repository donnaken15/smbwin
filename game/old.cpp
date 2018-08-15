#include <windows.h>
#include <windowsx.h>
#include "..\d3d8.h"

#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 480

#pragma comment (lib, "..\\d3d8.lib")
#pragma comment (lib, "..\\d3dx8.lib")

LPDIRECT3D8 d3d;
LPDIRECT3DDEVICE8 d3ddev;
LPDIRECT3DVERTEXBUFFER8 v_buffer = NULL;

void initD3D(HWND hWnd);
void render_frame(void);
void cleanD3D(void);
void init_graphics(void);

struct CUSTOMVERTEX { FLOAT X, Y, Z, RHW; DWORD COLOR; };
#define CUSTOMFVF (D3DFVF_XYZRHW | D3DFVF_DIFFUSE)

LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

/*HRESULT D3DXCreateTextureFromFile(
		LPDIRECT3DDEVICE9 pDevice,
		LPCTSTR pSrcFile,
		LPDIRECT3DTEXTURE9 *ppTexture
	);*/

int WINAPI WinMain(HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPSTR lpCmdLine,
	int nCmdShow)
{
	HWND hWnd;
	WNDCLASSEX wc;

	ZeroMemory(&wc, sizeof(WNDCLASSEX));

	wc.cbSize = sizeof(WNDCLASSEX);
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = WindowProc;
	wc.hInstance = hInstance;
	wc.hCursor = LoadCursor(NULL, NULL);
	wc.lpszClassName = L"GMBJ8P";
	ShowCursor(false);

	RegisterClassEx(&wc);

	hWnd = CreateWindowEx(NULL,
		L"GMBJ8P",
		L"Super Monkey Ball",
		WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX,
		0, 0,
		SCREEN_WIDTH, SCREEN_HEIGHT,
		NULL,
		NULL,
		hInstance,
		NULL);

	ShowWindow(hWnd, nCmdShow);

	initD3D(hWnd);

	MSG msg;

	while (TRUE)
	{
		while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		if (msg.message == WM_QUIT)
			break;

		render_frame();
	}

	cleanD3D();

	return msg.wParam;
}


LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_DESTROY:
	{
		PostQuitMessage(0);
		return 0;
	} break;
	}

	return DefWindowProc(hWnd, message, wParam, lParam);
}

void initD3D(HWND hWnd)
{
	
	d3d = Direct3DCreate8(D3D_SDK_VERSION);

	D3DPRESENT_PARAMETERS d3dpp;

	ZeroMemory(&d3dpp, sizeof(d3dpp));
	d3dpp.Windowed = TRUE;
	D3DSWAPEFFECT overidden_swapfx = D3DSWAPEFFECT_DISCARD;
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	d3dpp.hDeviceWindow = hWnd;
	_D3DFORMAT overridden_format = D3DFMT_X8R8G8B8;
	d3dpp.BackBufferFormat = overridden_format;
	d3dpp.BackBufferWidth = SCREEN_WIDTH;
	d3dpp.BackBufferHeight = SCREEN_HEIGHT;
	D3DDEVTYPE overridden_devtype = D3DDEVTYPE_HAL;
	d3d->CreateDevice(D3DADAPTER_DEFAULT,
		overridden_devtype,
		hWnd,
		D3DCREATE_SOFTWARE_VERTEXPROCESSING | D3DCREATE_MULTITHREADED,
		&d3dpp,
		&d3ddev);

	init_graphics();
}

void render_frame(void)
{
	d3ddev->Clear(0, NULL, D3DCLEAR_TARGET, D3DCOLOR_XRGB(0, 0, 0), 1.0f, 0);

	d3ddev->BeginScene();

	d3ddev->DrawPrimitive(D3DPT_TRIANGLELIST, 0, 1);

	d3ddev->ShowCursor(false);

	d3ddev->EndScene();

	d3ddev->Present(NULL, NULL, NULL, NULL);
}

void cleanD3D(void)
{
	v_buffer->Release();
	d3ddev->Release();
	d3d->Release();
}

void init_graphics(void)
{
	CUSTOMVERTEX vertices[] =
	{
		{ 0, 0, 0, 1, D3DCOLOR_XRGB(2550, 255, 255), },
		{ 640, 480, 0, 1.0f, D3DCOLOR_XRGB(0, 255, 0), },
		{ 0, 480, 0, 1.0f, D3DCOLOR_XRGB(255, 0, 0), },
	};

	d3ddev->CreateVertexBuffer(3 * sizeof(CUSTOMVERTEX),
		0,
		CUSTOMFVF,
		D3DPOOL_MANAGED,
		&v_buffer);

	VOID* pVoid;

	v_buffer->Lock(0, 0, (byte **)&pVoid, 0);
	memcpy(pVoid, vertices, sizeof(vertices));
	v_buffer->Unlock();
}