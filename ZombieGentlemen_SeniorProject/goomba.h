#pragma once
#include "entity.h"
#include <assert.h>

enum goombaStates{goombaWalkLeft, goombaWalkRight};

class goomba: public entity
{
private:
	goombaStates state;
	int sprite;
	float timer, flipTimer;
public:
	// constructor/destructor
	goomba();
	~goomba();

	//member functions
	void update(float timePassed);
	void animate();
	void flip();
	void reset();
	void setDirection(char dir);
};