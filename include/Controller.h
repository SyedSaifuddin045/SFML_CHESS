#pragma once
#include <GameModel.h>
#include <View.h>

class  Controller
{
public:
	Controller(int height, int width, const sf::String& title);
	void TogglePieceAt(sf::Vector2i mousePosition);
	~Controller();
	void RunGame();
private:
	GameModel model;
	View view;
};