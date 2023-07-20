#pragma once
#include <GameModel.h>
#include <View.h>

class  Controller
{
public:
	Controller(int height, int width, const sf::String& title);
	void TogglePiece(Piece& clickedPiece);
	void HandleInput(sf::RenderWindow& window);
	const std::vector<sf::Vector2i> GetPiecePositions(Piece& piece,sf::Vector2i boardPosition);
	~Controller();
	void RunGame();
private:
	GameModel model;
	View view;
};