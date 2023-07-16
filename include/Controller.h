#pragma once
#include <vector>

#include <Square.h>
#include <View.h>

class  Controller
{
public:
	Controller(int height,int width,const sf::String& title);
	void RunGame();
	~ Controller();

private:
	std::vector<Square> Board;
	View view;
};