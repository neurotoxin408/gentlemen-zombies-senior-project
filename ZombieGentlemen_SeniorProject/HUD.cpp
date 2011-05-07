#include "HUD.h"

HUD::HUD()
{
	dxMgr = NULL;
	//hudBackground = NULL;
	//barHolder = NULL;
	//barHolder2 = NULL;
	//healthBar = NULL;
	//armorBar = NULL;
	//weapon = NULL;
	//bagOfMoney = NULL;
	//PlayerID = NULL;
	PlayerMoney = NULL;
	images = NULL;
	plyr = NULL;
}
HUD::~HUD()
{
	if(images)
	{
		for(int i = 0; i < imgMgr->getSize(); i++)
		{
			if(images[i])
			{
				images[i]->~HudImage();
				images[i] = NULL;
			}
		}
	}
	delete [] images;
	images = NULL;

	// destroy player money
	if(PlayerMoney)
		PlayerMoney->~DXText();
	PlayerMoney = NULL;

	// destroy image manager (also destroys all images)
	imgMgr->~imageManager();
	imgMgr = NULL;

	//if(hudBackground)
	//	hudBackground->~HudImage();
	//hudBackground = NULL;
	//if(barHolder)
	//	barHolder->~HudImage();
	//barHolder = NULL;
	//if(barHolder2)
	//	barHolder2->~HudImage();
	//barHolder2 = NULL;
	//if(healthBar)
	//	healthBar->~HudImage();
	//healthBar = NULL;
	//if(armorBar)
	//	armorBar->~HudImage();
	//armorBar = NULL;
	//if(weapon)
	//	weapon->~HudImage();
	//weapon = NULL;
	//if(bagOfMoney)
	//	bagOfMoney->~HudImage();
	//bagOfMoney= NULL;
	//if(PlayerID)
	//	PlayerID->~HudImage();
	//PlayerID = NULL;

}
void HUD::draw()
{

	// draw all hud images
	if(images)
	{
		for(int i = 0; i < imgMgr->getSize(); i++)
		{
			if(images[i])
				images[i]->draw();
		}
	}

	// draw player money
	if(PlayerMoney)
		PlayerMoney->draw();

	//if(hudBackground)
	//	hudBackground->draw();
	//if(barHolder)
	//	barHolder->draw();
	//if(barHolder2)
	//	barHolder2->draw();
	//if(healthBar)
	//	healthBar->draw();
	//if(armorBar)
	//	armorBar->draw();
	//if(weapon)
	//	weapon->draw();
	//if(bagOfMoney)
	//	bagOfMoney->draw();
	//if(PlayerID)
	//	PlayerID->draw();

}
/****************************************************************
* loadFromFile
* takes in one text file, and loads images into an image manager
* from the image manager all HudImages are insansiated with image
* data
****************************************************************/
void HUD::loadFromFile(std::string filename, std::string textParam, dxManager * a_dxMgr)
{
	dxMgr = a_dxMgr;
	imgMgr = new imageManager();
	imgMgr->initImageManager(filename, dxMgr);


	// create as many HudImages as the number of images in the image manager
	images = new HudImage * [imgMgr->getSize()];

	// instansiate all the hudImages
	for(int i = 0; i < imgMgr->getSize(); i++)
	{
		images[i] = new HudImage(dxMgr, imgMgr->getImage(i), imgMgr->getImageInfo(i));
	}

	PlayerMoney		= new DXText(dxMgr);//DXText(dxMgr, imgMgr->getImage(8), imgMgr->getImageInfo(8));
	// after loading all the images, load text paramerts for all DXText objects
	PlayerMoney->loadFromTxtFile(textParam);
	PlayerMoney->setDialog("No Player");
	healthBar = images[imgMgr->getSize()-2];
	armorBar = images[imgMgr->getSize()-1];

		//hudBackground	= new HudImage(dxMgr, imgMgr->getImage(0), imgMgr->getImageInfo(0));
	//PlayerID		= new HudImage(dxMgr, imgMgr->getImage(1), imgMgr->getImageInfo(1));
	//barHolder		= new HudImage(dxMgr, imgMgr->getImage(2), imgMgr->getImageInfo(2));
	//healthBar		= new HudImage(dxMgr, imgMgr->getImage(3), imgMgr->getImageInfo(3));
	//barHolder2		= new HudImage(dxMgr, imgMgr->getImage(4), imgMgr->getImageInfo(4));
	//armorBar		= new HudImage(dxMgr, imgMgr->getImage(5), imgMgr->getImageInfo(5));
	//weapon			= new HudImage(dxMgr, imgMgr->getImage(6), imgMgr->getImageInfo(6));
	//bagOfMoney		= new HudImage(dxMgr, imgMgr->getImage(7), imgMgr->getImageInfo(7));
}
/****************************************************************
* update
* updates entire hud
* health bar is updated to illustrate Players current health
* armor bar is updated to illustrate Players current health
* money is updated to illustrate Players current funds
****************************************************************/
void HUD::update()
{
	if(plyr != NULL)
	{
		//healthBar->setParameters(plyr->getHealth(), healthBar->getHeight(), healthBar->getXPosition(), healthBar->getYPosition());
		//armorBar->setParameters(plyr->getArmor()->getArmorHealth(),armorBar->getHeight(), armorBar->getXPosition(), armorBar->getYPosition());

		//char updateBuffer[50];
		//sprintf_s(updateBuffer, "Money %.00f\n", plyr->getMoney());
		//PlayerMoney->setDialog(updateBuffer);
	}
}

/****************************************************************
* initDefaultPositions
* sets the default positions of the hud items
* also takes in an offset for the hud to move around the screen
****************************************************************/
void HUD::initPositions(std::string filename)
{
	std::fstream file(filename.c_str());

	file.peek();
	if(file.eof())
		MessageBox(NULL, "No HUD parameters", "HUD Error", MB_OK);
	// count the number of strings in the text file
	file.peek();
	int size = 0;
	while(!file.eof())
	{
		int c;
		c = file.get();
		if(c == '\n' || file.eof()) 
		{size++;}
	}
	// clear fstream flags
	file.clear();
	// set fstream get pointer back to the beginning
	file.seekg(0, std::ios::beg);

	if(size < imgMgr->getSize())
		MessageBox(NULL, "Missing HUD parameters", "HUD Error", MB_OK);

	// theses variables are used to load the parameters for each image
	float width, height, x, y;

	for(int i = 0; i < imgMgr->getSize(); i++)
	{
		file >> width >> height >> x >> y;
		images[i]->setParameters(width, height, x, y);
	}
	
	//hudBackground->setParameters(220.0f, 220.0f, 2.0f + a_x, 2.0f + a_y);
	//PlayerID->setParameters(70.0f, 70.0f, 19.0f + a_x, 27.0f + a_y);
	//barHolder->setParameters(124.0f, 100.0f, 82.0f + a_x, 9.0f + a_y);
	//healthBar->setParameters(100.0f, 12.0f, 95.0f + a_x, 43.0f + a_y);
	//barHolder2->setParameters(95.0f, 100.0f, 90.0f + a_x, 37.0f + a_y);
	//armorBar->setParameters(80.0f, 12.0f, 95.0f + a_x, 71.0f + a_y);	
	//weapon->setParameters(80.0f, 80.0f, 95.0f + a_x, 97.0f + a_y);
	//bagOfMoney->setParameters(75.0f, 75.0f, 22.0f + a_x, 95.0f + a_y);
}

/****************************************************************
* setPlayer
* sets the Player pointer
* must be called in order for the updates to take effect
****************************************************************/

void HUD::setPlayer(entity * a_Player){plyr = a_Player;}
