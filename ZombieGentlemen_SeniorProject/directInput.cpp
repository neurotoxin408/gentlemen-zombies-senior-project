/**
* see directInput.h for details
* TO-DO: make mouse exclusive at some point
*/

#include "directInput.h"

//#define MOUSE_EXCLUSIVE

directInput::directInput(void)
{
	g_lpDI = NULL;
	g_lpDIKeyboard = NULL;
	g_lpDIMouse =NULL;
	hr = NULL;
}

directInput::~directInput(void)
{
	shutdownDirectInput();
}

/*******************************************************************
* Initialize Direct Input
*******************************************************************/
bool directInput::initDirectInput(HWND * wndHandle, HINSTANCE * hInst)
{
	// Create the DirectInput object. 
    hr = DirectInput8Create(*hInst, DIRECTINPUT_VERSION, 
                            IID_IDirectInput8, (void**)&g_lpDI, NULL); 

	// Debuging
	if FAILED(hr) return FALSE; 
	if(hr == DIERR_INVALIDPARAM)
		MessageBox(NULL, "*** DirectInput8Create() FAILED *** Invalid Params", "Direct Input Error", MB_OK);
		else if(hr == DIERR_BETADIRECTINPUTVERSION)
		MessageBox(NULL, "*** DirectInput8Create() FAILED *** Beta Direct Input Version", "Direct Input Error", MB_OK);
		else if(hr == DIERR_OLDDIRECTINPUTVERSION)
		MessageBox(NULL, "*** DirectInput8Create() FAILED *** Old Direct Input Version", "Direct Input Error", MB_OK);
		else if(hr == DIERR_OUTOFMEMORY)
		MessageBox(NULL, "*** DirectInput8Create() FAILED *** Out of Memory", "Direct Input Error", MB_OK);
	if FAILED(hr) return FALSE; 

    // Retrieve a pointer to an IDirectInputDevice8 interface 
    hr = g_lpDI->CreateDevice(GUID_SysMouse, &g_lpDIMouse, NULL); 
	hr = g_lpDIMouse->SetDataFormat(&c_dfDIMouse); 
	
	if FAILED(hr) { 
		MessageBox(NULL, "*** Unable to create mouse device", "Direct Input Error", MB_OK);
		return FALSE; 
	} 

	hr = g_lpDI->CreateDevice(GUID_SysKeyboard, &g_lpDIKeyboard, NULL); 
	hr = g_lpDIKeyboard->SetDataFormat(&c_dfDIKeyboard); 


	if FAILED(hr) { 
		MessageBox(NULL, "*** Unable to keyboard mouse device", "Direct Input Error", MB_OK);
		return FALSE; 
	} 

	// xbox controller
	xcont = new XboxController(1);


#ifdef MOUSE_EXCLUSIVE
	// Set the cooperative level 
    hr = g_lpDIMouse->SetCooperativeLevel(*wndHandle, 
                             DISCL_FOREGROUND | DISCL_EXCLUSIVE);
#else
	// Set the cooperative level 
    hr = g_lpDIMouse->SetCooperativeLevel(*wndHandle, 
                             DISCL_FOREGROUND | DISCL_NONEXCLUSIVE); 
#endif
	
	if FAILED(hr) { 
		MessageBox(NULL, "*** Unable to set mouse cooperative level", "Direct Input Error", MB_OK);
		return FALSE; 
	} 

	// Set the cooperative level 
    hr = g_lpDIKeyboard->SetCooperativeLevel(*wndHandle, 
                             DISCL_FOREGROUND | DISCL_NONEXCLUSIVE); 

    if FAILED(hr) 
    { 
		MessageBox(NULL, "*** Unable to keyboard cooperative level", "Direct Input Error", MB_OK);
        return FALSE; 
    } 

    // Get access to the input device. 
	do
	{
		hr = g_lpDIKeyboard->Acquire();
	}
	while FAILED(hr);

	do
	{
		hr = g_lpDIMouse->Acquire();
	}
	while FAILED(hr);
	
    if FAILED(hr) 
    { 
		MessageBox(NULL, "*** Unable to acquire devices", "Direct Input Error", MB_OK);
        return FALSE; 
    }
	else
		return TRUE;
}

bool directInput::updateMouseState()
{
	hr = g_lpDIMouse->GetDeviceState(sizeof(mouseState),(LPVOID)&mouseState);
	 if FAILED(hr) 
    { 
        return FALSE; 
    }
	 else
		 return TRUE;
}
bool directInput::updateKeyboardState()
{
	hr = g_lpDIKeyboard->GetDeviceState(sizeof(keyState),(LPVOID)&keyState);
	if FAILED(hr) 
    { 
        return FALSE; 
    }
	else
		return TRUE;
}

BYTE * directInput::getKeyboardState()
{
	return keyState;
}
DIMOUSESTATE * directInput::getMouseState()
{
	return &mouseState;
}
XboxController * directInput::getXboxController()
{
	return xcont;
}

void directInput::reAcquireDevices()
{
	// try and reacquire the input device
	hr = g_lpDIKeyboard->Acquire();
	// do a continuous loop until the device is reacquired
	while( hr == DIERR_INPUTLOST ) 
			hr = g_lpDIKeyboard->Acquire();

	// try and reacquire the input device
	hr = g_lpDIMouse->Acquire();
	// do a continuous loop until the device is reacquired
	while( hr == DIERR_INPUTLOST ) 
			hr = g_lpDIMouse->Acquire();

}

void directInput::shutdownDirectInput(void)
{
	if (g_lpDI) 
    { 
		if (g_lpDIMouse) 
        { 
        // Always unacquire device before calling Release(). 
            g_lpDIMouse->Unacquire(); 
            g_lpDIMouse->Release();
            g_lpDIMouse = NULL; 
        } 
        if (g_lpDIKeyboard) 
        { 
        // Always unacquire device before calling Release(). 
            g_lpDIKeyboard->Unacquire(); 
            g_lpDIKeyboard->Release();
            g_lpDIKeyboard = NULL; 
        } 
        g_lpDI->Release();
        g_lpDI = NULL; 
    }
}