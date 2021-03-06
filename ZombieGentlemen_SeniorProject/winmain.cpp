// includeFiles.h holds all other include headers nessary for this game
#include <Windows.h>
#include "game.h"
/*******************************************************************************************************
* Windows
* The following code creates a window
********************************************************************************************************/

HINSTANCE hInstance; // global handle to hold the application instance
HWND wndHandle; //global variable to hold the window handle

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600

// forward declarations
bool initWindow( HINSTANCE hInstance);
LRESULT CALLBACK WndProc( HWND, UINT, WPARAM, LPARAM );

// This is winmain, the main entry point for Windows applications
int WINAPI WinMain ( HINSTANCE hInstance, HINSTANCE hPrevInstance, 
					LPTSTR lpCmdLine, int nCmdShow )
{
	//Initialize the window
	if (!initWindow(hInstance))
	{
		MessageBox(NULL, "Unable to create window", "Windows Error", MB_OK);
		return false;
	}

	dxManager * dxMgr = new dxManager();
	// Initialzie DirectX Manager
	if(!dxMgr->initDirect3D(&wndHandle, &hInstance))
	{
		MessageBox(NULL, "Unable to initialize Direct3D", "Direct3D Error", MB_OK);
		return false;
	}

	directInput * inputMgr = new directInput();
	// initialize Direct Input
	if(!inputMgr->initDirectInput(&wndHandle, &hInstance))
	{
		MessageBox(NULL, "Unable to initialize Direct Input", "Direct Input Error", MB_OK);
		return false;
	}

	sound * soundMgr = new sound();
	// initialize Direct Sound
	if(!soundMgr->initDirectSound(&wndHandle))
	{
		MessageBox(NULL, "Unable to initialize Direct Sound", "Direct Sound Error", MB_OK);
		return false;
	}

	game * g = new game(&wndHandle, &hInstance);
	if(!g->initGame(dxMgr, inputMgr, soundMgr))
	{
		MessageBox(NULL, "Unable to initialize game", "Game Error", MB_OK);
		return false;
	}
	
	//main message loop
	MSG msg;
	ZeroMemory( &msg, sizeof( msg ) );

	// Game/Windows loop
	while( msg.message!=WM_QUIT )
	{
	
		if( PeekMessage( &msg, NULL, 0U, 0U, PM_REMOVE))
		{
			TranslateMessage ( &msg );
			DispatchMessage( &msg );
		}
		else
		{
			g->update();
		}
	}
	delete g;
	dxMgr = NULL;
	inputMgr = NULL;
	soundMgr = NULL;
	return 0;
}

/*********************************************************************
* bool initWindow( HINSTANCE hInstance )
* initWindow registers the window calss for the application. creates the window
**********************************************************************/
bool initWindow( HINSTANCE hInstance )
{
	WNDCLASSEX wcex;
	memset(&wcex, 0, sizeof(wcex));

	// this code loads the icon into a SHFILEINFO
	string icon = "gameIcon.ico";
	SHFILEINFO shfi;
	SHGetFileInfo( icon.c_str(), 0, &shfi, sizeof( SHFILEINFO ), 
               SHGFI_ICON | SHGFI_LARGEICON);

	// Fill the WNDCLASSEX structure. This describes how the window ill look to the system
	wcex.cbSize			= sizeof(WNDCLASSEX);		// the sixe of the structure
	wcex.style			= CS_HREDRAW | CS_VREDRAW;  // the class style
	wcex.lpfnWndProc	= (WNDPROC)WndProc;			// the window procedure callback
	wcex.cbClsExtra		= 0;						// extra bytes to allocate for this class
	wcex.cbWndExtra		= 0;						// extra bytes to allocate for this instance
	wcex.hInstance		= hInstance;				// handle to the application instance
	//wcex.hIcon		= shfi.hIcon;				// the large icon image data is possed into the window
	//wcex.hIconSm	    = shfi.hIcon;				// the small icon image data is possed into the window
	wcex.hIcon			= ::LoadIcon( NULL, IDI_APPLICATION );		// the large icon image data is possed into the window
	wcex.hIconSm	    = NULL;				// the small icon image data is possed into the window
	wcex.hCursor		= ::LoadCursor(NULL, IDC_ARROW); // the default cursor
	wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1); // the background color
	wcex.lpszMenuName	= NULL;						// the resource name for the menu
	wcex.lpszClassName	= "GSP Senior Project";		// the class name being created


	if(!RegisterClassEx(&wcex))
	{
		char buffer[256];
		memset(buffer, 0, sizeof(buffer));
		sprintf_s(buffer, "RegisterClassEx() failed, error number: %i", GetLastError());

		MessageBox(NULL, buffer, "Windows Error", MB_OK);
		return false;
	}

	// Create the window
	wndHandle = CreateWindow(
				"GSP Senior Project",			// the window class to use
				"Tiny Steve Missing An Eye",	// the title bar text
				WS_OVERLAPPEDWINDOW,			// the window style
				CW_USEDEFAULT,					// the starting x coordinate
				CW_USEDEFAULT,					// the startying y coordinate
				WINDOW_WIDTH,					// the pixel width of the window
				WINDOW_HEIGHT,					// the pixel height of the window
				NULL,							// the parent window NULL for desktop
				NULL,							// The menu for the application; NULL for none
				hInstance,						// the handle to the applicatilon instance
				NULL);							// no values passed to the window

	// make sure that the window handle that is created is valid
	if(!wndHandle)
	{
		char buffer[256];
		memset(buffer, 0, sizeof(buffer));
		sprintf_s(buffer, "CreateWindow() failed, error number: %i", GetLastError());

		MessageBox(NULL, buffer, "Windows Error", MB_OK);
		return false;
	}

	// Display the window on the screen
	ShowWindow(wndHandle, SW_SHOW);

	UpdateWindow(wndHandle);
	return true;
}

/***************************************************************************
* LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
*
* The window procedure
****************************************************************************/
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	// Check any available message from the queue 
	switch (message)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	}
	// Always return the message to the default window
	// procedure for further processing
	return DefWindowProc(hWnd, message, wParam, lParam);
}