/**
* dxManager handles the 2D graphics for our game
* status: running
*/

#pragma once

#define DIRECTINPUT_VERSION 0x0800
#define RES_WIDTH 1920
#define RES_HEIGHT 1080

#include <d3d9.h>
#include <d3dx9tex.h>
#include <dinput.h>
#include <string>

class dxManager
{
public:
	dxManager(void);
	~dxManager(void);
	bool initDirect3D(HWND * wndHandle, HINSTANCE * hInst);
	void shutdown(void);
	void beginRender(void);
	void endRender(void);
	IDirect3DSurface9* getSurfaceFromBitmap(std::string filename, int width, int height);
	IDirect3DSurface9* getAlphaSurfaceFromBitmap(std::string filename, int width, int height, D3DCOLOR ColorKey);
	IDirect3DSurface9* getBackBuffer(void);
	void blitToSurface(IDirect3DSurface9* srcSurface, const RECT *srcRect, const RECT *destRect);
	LPDIRECT3DDEVICE9 * getDevice(void);

private:
	HRESULT                 lastResult;
	LPDIRECT3D9             pD3D;
	LPDIRECT3DDEVICE9       pd3dDevice;
};
