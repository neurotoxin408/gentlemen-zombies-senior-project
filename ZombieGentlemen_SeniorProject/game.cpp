#include "game.h"

game::game(HWND * a_wndHandle, HINSTANCE * a_hInstance)
{
	m_hInstance = a_hInstance;
	m_wndHandle = a_wndHandle;
}
bool game::initGame(dxManager * a_dxMgr, directInput * a_inputMgr, sound * a_soundMgr)
{
	// initiallize game data
	dxMgr = a_dxMgr;
	inputMgr = a_inputMgr;
	soundMgr = a_soundMgr;
	
	// set game initial state
	gameState = menu;

	// initialize timer data
	now = clock();
	//then = now;
	//passed = now-then;

	QueryPerformanceFrequency(&timerFreq);
	UpdateSpeed = 0;

	// initialize input data
	input = new inputData;
	input->init(inputMgr->getXboxController());

	// initiailize camera
	camera = new dxCamera(dxMgr);

	// initialize loading screen
	loadingScreenImage = new HudImage(a_dxMgr, "images/loadingscreen.bmp");
	loadingScreenImage->setSize(1920.0, 1100.0);
	loadingScreen = new DXText(a_dxMgr, "images/BlackTextBox.bmp");
	loadingScreen->loadFromTxtFile("loadingScreen.txt");
	loadingScreen->setDialog("  Loading...");
	loadingScreen->draw();

	// init controller debug data
	controllerDebug = new DXText(a_dxMgr, "images/BlackTextBox.bmp");
	controllerDebug->loadFromTxtFile("textParameters2.txt");
	controllerDebug->setDialog("Loading...");
	
	//instantiate menu (TEST DATA)
	mainMenu = new Menu(dxMgr,"MenuArt.txt","options.txt");
	mainMenu->setParam(RES_WIDTH, RES_HEIGHT, 0, 0);
	//mainMenu->setParam(WINDOW_WIDTH/2,WINDOW_HEIGHT/2,WINDOW_WIDTH/4,WINDOW_HEIGHT/4);

	// initialize town (TEST DATA)
	the_town = new town();
	the_town->init(dxMgr, camera, "anotherInit.txt","coords2.txt");

	// initialize sound data (TEST DATA)
	a_soundMgr->initSoundFiles("soundManager.txt");

	lvlMgr = new levelManager;
	lvlMgr->init("levels.txt", dxMgr, camera, soundMgr);
	return true;
}

void game::update()
{
	//update time
	now = clock();
	QueryPerformanceCounter(&timeStart);
	
	// Handle Input using Direct Input
	handleInput();
	// update according to game state
	switch(gameState)
	{
	case menu:
		soundMgr->playSoundLoop(soundMenu);
		break;
	case sideScroll:
			if(!lvl->update(UpdateSpeed))
				gameState = topDown;
		break;
	case topDown:
		// top down level one message
		break;
	}

	if(input->xConnectTimer < xControllerTimout)
		input->xConnectTimer += UpdateSpeed;

	// draw to the screen using Direct3D
	draw();

	// update high resolution timer
	QueryPerformanceCounter(&timeEnd);
	UpdateSpeed = ((float)timeEnd.QuadPart - (float)timeStart.QuadPart)/
		timerFreq.QuadPart;
}

void game::handleInput()
{
	inputMgr->reAcquireDevices();
	inputMgr->updateKeyboardState(); 
	input->keystate = inputMgr->getKeyboardState();
	inputMgr->updateMouseState();
	input->mouseState = *(inputMgr->getMouseState());


	// update controller prompt
	if(input->xcont->IsConnected())
	{
		if(!input->xConnected)
		{
			char Buffer[256];
			sprintf_s(Buffer, "Controller Connected");
			controllerDebug->setDialog(Buffer);
			input->xConnected = true;
			input->xConnectTimer = 0;	
		}
	}
	else
	{
		if(input->xConnected)
		{
			char Buffer[256];
			sprintf_s(Buffer, "Controller Disconnected");
			controllerDebug->setDialog(Buffer);
			input->xConnected = false;
			input->xConnectTimer = 0;	
		}
	}


	// if esc is pressed Quit Game globally
	if(input->keystate[DIK_ESCAPE] & 0x80)
	{
		PostQuitMessage(0);
	}

	int check = 0;

	switch(gameState)
	{
	case menu:
		check = mainMenu->update(input, now);
		if(check == 1)
		{
			mainMenu->~Menu();
			//gameState = sideScroll;

			gameState = topDown;
			soundMgr->stopSound(soundLevel);
			soundMgr->playSound(soundMenu);
		}
		if(check == 2)
		{
			// load game will go here
			PostQuitMessage(0);
		}
		if(check == 3)
		{
			PostQuitMessage(0);
		}
		
		break;
	case topDown:
		{
			int the_level = the_town->update(input,now);
			const int no_level = -1;
			if(the_level != no_level)
			{
				gameState = loading;
				draw();
				lvlMgr->setLevel(the_level); 
				lvl = lvlMgr->getLevel();
				gameState = sideScroll;
			}
		}
		break;

	case sideScroll:

		// when game is in level state, input is handled inside level
		lvl->handleInput(input, now);
		break;
	}
}

void game::draw()
{
	dxMgr->beginRender();

	switch(gameState)
	{
	case menu:
		mainMenu->Draw();
		break;
	case topDown:
		the_town->draw();
		break;
	case sideScroll:
			lvl->draw();
		break;
	case loading:
		loadingScreenImage->draw();
		loadingScreen->draw();
		break;
	}

	// draw controller debug data
	if(input->xConnectTimer <= xControllerTimout && gameState != loading)
		controllerDebug->draw();

	dxMgr->endRender();
}
game::~game()
{	
	// destroy the loading screen
	delete loadingScreen;
	loadingScreen = NULL;
	loadingScreenImage->~HudImage();

	//destroy the level
	delete lvlMgr;
	lvlMgr = NULL;
	//destry the town
	delete the_town;
	the_town = NULL;

	// destroy controller debug
	delete controllerDebug;
	controllerDebug = NULL;
	//destroy input data
	delete input;
	input = NULL;
	// destroy the camera
	delete camera;
	camera = NULL;
	// destroy direct3D data
	delete dxMgr;
	dxMgr = NULL;
	// destroy direct input data
	delete inputMgr;
	inputMgr = NULL;
	// desyroy direct sound data
	delete soundMgr;
	soundMgr = NULL;
}