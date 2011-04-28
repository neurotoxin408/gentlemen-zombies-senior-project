#include "Menu.h"
/*
TODO: make a few more constructors, make it less magic numbery, figure out how to make a Menu within a menu
*/

Menu::Menu()
{
	selected = 0;
	imgMan = NULL;
	background = NULL;
	option_txt = NULL;
	options = NULL;
}
Menu::Menu(dxManager * a_dxMgr,char* menuArt,char* menuOptions)//takes the the directx manager and initialized everything
{
	//creates a new image manager from a txt file and creates a background image out of the first element
	selected = 0;
	imgMan = new imageManager();
	imgMan->initImageManager(menuArt,a_dxMgr);
	background = new HudImage(a_dxMgr,imgMan->getImage(0),imgMan->getImageInfo(0));

	//is a string array that reads a file of all the dialog and creates options
	option_txt = new stringArray;
	option_txt->loadFromTextFile(menuOptions);

	//creates text boxes out of the information given from image manager
	options = new DXText* [option_txt->getSize()];
	for(int c=0;c<option_txt->getSize();++c)
	{
		options[c] = new DXText(a_dxMgr,imgMan->getImage(1),imgMan->getImageInfo(1));
		options[c]->setDialog(option_txt->getStringAt(c));
	}
	//default parameters
	const float height = 300, width = 300;
	setParam(height,width,640/2,480/2);
}

void Menu::Draw()
{
	background->draw();
	for(int a=0;a<option_txt->getSize();++a)
		options[a]->draw();
}

void Menu::dialogueBox()
{

}
void Menu::setParam(float w, float h, float a_x, float a_y)
{
	background->setParameters(w,h,a_x,a_y);
	for(int q=0;q<option_txt->getSize();++q)
	{
		//offset seemingly random
		options[q]->setTextBoxParameters(w/2,h/4,a_x,a_y+(q*100),10);
		options[q]->setFontColor(0xff,0x00,0x00,0x00);//sets the color to black in hex
	}
}
void Menu::questDialogue(std::string filename)
{

}
void Menu::storeMenu(std::string filename)
{

}
int Menu::update(BYTE* keystate,int now,int * keylag)
{
	
	//key allows you to scroll through the options
	if(keystate[DIK_T] & 0x80)
	{
		if(now - keylag[DIK_T] > 150)
		{
			if (selected == 0) 
			{
				selected++;
				options[selected-1]->getimg()->shareImage(imgMan->getImage(2),imgMan->getImageInfo(2));
			}
			else if(selected < option_txt->getSize())
			{
				options[selected-1]->getimg()->shareImage(imgMan->getImage(1),imgMan->getImageInfo(1));
				options[selected]->getimg()->shareImage(imgMan->getImage(2),imgMan->getImageInfo(2));
				selected++;
			}
			else
			{
				options[selected-1]->getimg()->shareImage(imgMan->getImage(1),imgMan->getImageInfo(1));
				selected = 0;
			}
			keylag[DIK_T] = now;
		}
		
	}
	//key that says that an option was hit
	if(keystate[DIK_E] & 0x80)
	{
		return selected;
	}
	return 0;
}
Menu::~Menu()
{
	imgMan->~imageManager();

}