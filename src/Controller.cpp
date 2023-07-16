#include <Controller.h>

Controller::Controller(int height,int width,const sf::String& title)
	:view(sf::VideoMode(width,height),title)
{
}

void Controller::RunGame()
{
	view.Display();
}

Controller::~Controller()
{
}