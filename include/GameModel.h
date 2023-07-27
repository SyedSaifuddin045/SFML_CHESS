#pragma once
#include <stack>
#include <vector>
#include <memory>
#include <Tile.h>
#include <PieceFactory.h>
#include <MoveCommand.h>

namespace std {
	template<>
	struct hash<sf::Vector2i>
	{
		size_t operator()(const sf::Vector2i& key)const
		{
			return hash<int>()(key.x) ^ hash<int>()(key.y);
		}
	};
}

class Player;

class GameModel {
public:

	GameModel();
	std::pair<Global::Piece_Type, Global::Color> GetPieceTypeAndColorAt(int row, int col);
	std::shared_ptr<Piece> getPieceAtPosition(sf::Vector2i pos);
	void InitializeBoard();
	std::vector<std::vector<Tile>>& getBoard() { return this->Board; }
	bool isOccupied(sf::Vector2i position);
	bool isPositionOccupiedByEnemy(sf::Vector2i position, Global::Color ourColor);
	std::pair<sf::Vector2i, std::shared_ptr<Piece>> MovePiece(std::shared_ptr<Piece> piece, sf::Vector2i position, std::pair<sf::Vector2i, std::shared_ptr<Piece>> capturePair);
	void executeCommand(std::unique_ptr<Command> command);
	const std::vector<sf::Vector2i> GetPiecePositions(std::shared_ptr<Piece> piece, sf::Vector2i boardPosition);
	void redoCommand();
	void undoLastCommand();
	std::unordered_map<sf::Vector2i, std::shared_ptr<Piece>>& getPositioOccupiedOnBoard();
	const int rows = 8;
	const int cols = 8;
	const float tile_size = 80.0f;

	std::shared_ptr<sf::Texture> WhiteTexture;
	std::shared_ptr<sf::Texture> BlackTexture;
	std::shared_ptr<sf::Texture> WhiteHighlightTexture;
	std::shared_ptr<sf::Texture> BlackHighlightTexture;
	friend Player;
private:
	std::vector<std::vector<Tile>> Board;
	std::unordered_map<sf::Vector2i, std::shared_ptr<Piece>> positionsOccupiedOnBoard;
	std::stack<std::unique_ptr<Command>> executedCommands;
	std::stack<std::unique_ptr<Command>> undoneCommands;
};