#include "turret.h"

turret::turret()
{
	time = 0;
}
turret::~turret()
{

}

void turret::update(float timePassed)
{
	time += timePassed;
	if(time <= 1.0f)
	{
		switch(state)
		{
		case fortyFiveRight:
			m_object->setSprite(0, 0);
			state = nindy;
			break;

		case nindy:
			m_object->setSprite(0, 1);
			state = fortyFiveLeft;
			break;

		case fortyFiveLeft:
			m_object->setSprite(0, 2);
			state = fortyFiveRight;
			break;
		}
	}

	m_object->getCollisionRect()->update();
}