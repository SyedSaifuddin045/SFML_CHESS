#pragma once

#include <iamgui/imgui.h>
#include <iamgui/imgui-SFML.h>

#include <GameModel.h>
#include <View.h>
#include <Player.h>

class  Controller
{
public:
	Controller(int height, int width, const sf::String& title);
	void TogglePiece(std::shared_ptr<Piece> clickedPiece, sf::Vector2i pos);
	void HandleInput(sf::RenderWindow& window);
	void ResetTiles();
	void PreventDuplicateClicks(int rowIndex, int columnIndex, bool& retFlag);
	~Controller();
	void RunGame();
	void ChangePlayer();
private:
	GameModel model;
	View view;
	Tile* lastClickedTile;
	std::shared_ptr<Piece> lastClickedPiece;
	sf::Vector2i lastClickedPiecePosition;

	sf::Vector2i lastMouseClickPosition;
	std::vector<sf::Vector2i> ClickedPiecePositions;
	Global::Player WhitePlayer;
	Global::Player BlackPlayer;
	Global::Player* currentPlayer;
	bool isPaused;
};