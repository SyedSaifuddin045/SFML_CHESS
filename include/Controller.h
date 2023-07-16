#pragma once
#include <vector>

#include <Tile.h>
#include <View.h>

constexpr auto rows = 8;
constexpr auto cols = 8;

class  Controller
{
public:
	Controller(int height, int width, const sf::String& title);
	void RunGame();
	~Controller();
	void InitializeBoard();
private:
	std::vector<std::vector<Tile>> Board;
	sf::Texture WhiteTexture;
	sf::Texture BlackTexture;
	View view;
};