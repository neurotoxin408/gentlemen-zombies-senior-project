#pragma once
#include "dxManager.h"
#include "objectManager.h"
#include "inputData.h"
#include "directInput.h"
#include "physics.h"
#include "stringArray.h"
#include "background.h"
#include "dxCamera.h"
#include "HUD.h"
#include "entityManager.h"

class level
{
private:
	objectManager * objMgr;
	stringArray * files;
	cubeMap * m_map;
	background * backGrnd;
	HUD * p1HUD;
	DXText* checkpointtxt;
	dxCamera * camera;
	int checkpoint;
	entityManager * entityMgr;
	player * m_player;


	float Elapsed;
	int FPS;
	// debug data
	DXText * FPSText;
	DXText * physicsData; 

public:
	level();
	void initLevel(dxManager* a_dxMgr, dxCamera * a_camera, std::string filename);
	void setMusic(char*);//sets the music for the level
	void draw();
	void handleInput(inputData * input, int now);
	void update(float timePassed);
	void updateCamera();
	void updateDebugData(float updateTime);
	~level();//deconstructor
	bool hitcheckpoint();
	void loadfromcheckpoint(std::string);
	//debug code
	void pop();
	void moveObject(D3DXVECTOR3);//may or maynot stay that way
	object* getobject(){return objMgr->getObject();}
	objectManager* getManager(){return objMgr;}
};