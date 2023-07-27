#include <unordered_map>
#include <functional>

#include "Piece.h"
#include "GameModel.h"

namespace Global {
	class Player
	{
	public:
		Player(sf::Vector2i kingPosition,GameModel& model);
		Color getColor() { return this->color; }
		bool isInCheck();
		void updateKingPosition(sf::Vector2i pos);
		void checkForCheck();
		void CheckDiagonal();
		void CheckStraight();
		void CheckPyada();
		std::unordered_map<std::shared_ptr<Piece>, std::vector<sf::Vector2i>>& getValidMoves();
		bool KingMustMove();
		bool isPiecePinned(std::shared_ptr<Piece> piece);
		void CheckGhoda();
		void getAllVaidMoves();
		void checkMate();
	private:
		Global::Color color;
		sf::Vector2i kingPosition;
		std::shared_ptr<Piece> King;
		bool isCheck;

		std::vector<std::pair<sf::Vector2i, std::shared_ptr<Piece>>> CheckPieces;
		std::vector<std::pair<sf::Vector2i, std::shared_ptr<Piece>>> PinnedPieces;
		std::unordered_map<std::shared_ptr<Piece>, std::vector<sf::Vector2i>> validMoves;

		GameModel& model;
	};
}