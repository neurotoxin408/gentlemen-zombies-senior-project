#pragma once

#include "entity.h"

enum playerState{idle, walkingRight, walkingLeft, jumping, dying, dead};
enum playerSprite{playerSprite1, playerSprite2, playerSprite3, playerSprite4, playerSprite5};

class player: public entity
{
private:
	playerState state;
	playerSprite sprites;
	float timer, armorTimeout, randomIdle;
	bool armorBlink;

public:
	// constructor/destructor
	player();
	~player();

	// member functions
	void animate();
	void reset();
	void update(float timePassed);
	void move(float x, float y);
	void bounce();
	void removeArmor();
	bool getAmorBlink();
	void armorPickup();
};