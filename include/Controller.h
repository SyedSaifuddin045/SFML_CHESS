#pragma once
#include <GameModel.h>
#include <View.h>

class  Controller
{
public:
	Controller(int height, int width, const sf::String& title);
	void TogglePiece(std::shared_ptr<Piece> clickedPiece, sf::Vector2i pos);
	void HandleInput(sf::RenderWindow& window);
	void ResetTiles();
	void PreventDuplicateClicks(int rowIndex, int columnIndex, bool& retFlag);
	const std::vector<sf::Vector2i> GetPiecePositions(std::shared_ptr<Piece> piece,sf::Vector2i boardPosition);
	~Controller();
	void RunGame();
private:
	GameModel model;
	View view;
	Tile* lastClickedTile;
	std::shared_ptr<Piece> lastClickedPiece;
	sf::Vector2i lastClickedPiecePosition;

	sf::Vector2i lastMouseClickPosition;
	std::vector<sf::Vector2i> ClickedPiecePositions;
};