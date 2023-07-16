#pragma once
#include <GameModel.h>
#include <View.h>

class  Controller
{
public:
	Controller(int height, int width, const sf::String& title);
	~Controller();
	void RunGame();
private:
	GameModel model;
	View view;
};