#pragma once

#include <iamgui/imgui.h>
#include <iamgui/imgui-SFML.h>

#include <GameModel.h>
#include <View.h>
#include <Player.h>
#include <Server.h>
#include <Client.h>

enum class GameState{
	MainMenu,WaitingForConnection, PlayGame, Pause, End
};

class  Controller
{
public:
	Controller(int height, int width, const sf::String& title);
	void SetUpController();
	void TogglePiece(std::shared_ptr<Piece> clickedPiece, sf::Vector2i pos);
	void HandleInput(sf::RenderWindow& window,bool isKingCheck);
	void ResetTiles();
	void PreventDuplicateClicks(int rowIndex, int columnIndex, bool& retFlag);
	~Controller();
	void CloseGame();
	void RunGame();
	void ShowMainMenu(bool& showOnlineOption);
	void ShowPlayGameScreen(bool& undoKeyPressed, bool& redoKeyPressed, sf::RenderWindow& window);
	void ShowEndScreen(sf::RenderWindow& window);
	void ShowTurnText(sf::RenderWindow& window);
	void ShowUndoAndRedoButtons(sf::RenderWindow& window);
	void showPauseOrPlayButton();
	void ShowTextInCentre(sf::RenderWindow& window, std::string s);
	void ChangePlayer();
	void CreateServer(unsigned short port);
	void CreateClient(const char* address, unsigned short port);
	void WaitForConnection();
	void ShowSpinner();
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
	bool checkMate;
	GameState gameState;

	bool playOnline;
	Client* client;
	Server* server;
};