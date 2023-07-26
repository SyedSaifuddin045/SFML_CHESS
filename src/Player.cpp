#include "Player.h"

Global::Player::Player(sf::Vector2i kingPosition, GameModel& model)
	:kingPosition(kingPosition), model(model), King(model.getPieceAtPosition(kingPosition))
{
	color = King->getPieceColor();
	isCheck = false;
}

bool Global::Player::isInCheck()
{
	CheckPieces.clear();
	PinnedPieces.clear();
	isCheck = false;
	checkForCheck();
	/*if (isCheck)
	{
		for (auto CheckPieces : CheckPieces)
		{
			std::cout << Global::GetStringFromColor(color) << " king is check from : " << Global::GetStringFromPieceType(CheckPieces.second->getPieceType()) << std::endl;
		}
	}
	for (auto pinnedPiece : PinnedPieces)
	{
		std::cout << "Piece " << Global::GetStringFromPieceType(pinnedPiece.second->getPieceType()) << " is pinned at : " << pinnedPiece.first.x << "," << pinnedPiece.first.y << std::endl;
	}*/
	return this->isCheck;
}


void Global::Player::updateKingPosition(sf::Vector2i pos)
{
	kingPosition = pos;
	CheckPieces.clear();
	PinnedPieces.clear();
}

void Global::Player::checkForCheck()
{
	CheckGhoda();

	CheckPyada();

	CheckStraight();

	CheckDiagonal();
}

void Global::Player::CheckDiagonal()
{
	int directions[4][2] = { { 1, 1 },{ 1, -1 },{ -1, 1 },{ -1, -1 } };

	for (const auto& dir : directions) {
		int dx = dir[0];
		int dy = dir[1];

		int x = kingPosition.x + dx;
		int y = kingPosition.y + dy;

		int friendlyCount = 0;
		std::pair<sf::Vector2i, std::shared_ptr<Piece>> lastFriendly;

		while (x >= 0 && x < 8 && y >= 0 && y < 8) {
			if (model.isPositionOccupiedByEnemy(sf::Vector2i(x, y), color)) {
				auto pieceAtPosition = model.getPieceAtPosition(sf::Vector2i(x, y));

				if (pieceAtPosition->getPieceType() == Global::Piece_Type::Unth || pieceAtPosition->getPieceType() == Global::Piece_Type::Wazir) {
					if (friendlyCount >= 1)
					{
						PinnedPieces.push_back(lastFriendly);
						break;
					}
					isCheck = true;
					CheckPieces.push_back(std::make_pair(sf::Vector2i(x, y), pieceAtPosition));
				}
				break;
			}
			else if (model.isOccupied(sf::Vector2i(x, y)) && model.getPieceAtPosition(sf::Vector2i(x, y))->getPieceColor() == color)
			{
				friendlyCount++;
				if (friendlyCount == 2)
					break;
				lastFriendly = std::make_pair(sf::Vector2i(x, y), model.getPieceAtPosition(sf::Vector2i(x, y)));
			}
			x += dx;
			y += dy;
		}
		friendlyCount = 0;
	}
}

void Global::Player::CheckStraight()
{
	int directions[4][2] = { { 1, 0 },{ -1, 0 },{ 0, 1 },{ 0, -1 } };

	for (const auto& dir : directions) {
		int dx = dir[0];
		int dy = dir[1];

		int x = kingPosition.x + dx;
		int y = kingPosition.y + dy;

		int friendlyCount = 0;
		std::pair<sf::Vector2i, std::shared_ptr<Piece>> lastFriendly;

		while (x >= 0 && x < 8 && y >= 0 && y < 8) {
			if (model.isPositionOccupiedByEnemy(sf::Vector2i(x, y), color)) {
				auto pieceAtPosition = model.getPieceAtPosition(sf::Vector2i(x, y));

				if (pieceAtPosition->getPieceType() == Global::Piece_Type::Haanthi || pieceAtPosition->getPieceType() == Global::Piece_Type::Wazir) {
					if (friendlyCount >= 1)
					{
						PinnedPieces.push_back(lastFriendly);
						break;
					}
					isCheck = true;
					CheckPieces.push_back(std::make_pair(sf::Vector2i(x, y), pieceAtPosition));
				}
				break;
			}
			else if (model.isOccupied(sf::Vector2i(x, y)) && model.getPieceAtPosition(sf::Vector2i(x, y))->getPieceColor() == color) {
				friendlyCount++;
				if (friendlyCount == 2)
					break;
				lastFriendly = std::make_pair(sf::Vector2i(x, y), model.getPieceAtPosition(sf::Vector2i(x, y)));
			}
			x += dx;
			y += dy;
		}

		// Reset the friendly count for the next straight direction
		friendlyCount = 0;
	}
}


void Global::Player::CheckPyada()
{
	int Direction = (color == Global::Color::white) ? -1 : +1;

	for (int y_offset : {-1, 1})
	{
		int new_y = kingPosition.y + y_offset;
		int new_x = kingPosition.x + (1 * Direction);

		if (new_y >= 0 && new_y < 8 && model.isPositionOccupiedByEnemy(sf::Vector2i(new_x, new_y), color))
		{
			auto pieceAtPosition = model.getPieceAtPosition(sf::Vector2i(new_x, new_y));
			if (pieceAtPosition->getPieceType() == Piece_Type::Pyada)
			{
				isCheck = true;
				CheckPieces.push_back(std::make_pair(sf::Vector2i(new_x, new_y), pieceAtPosition));
			}
		}
	}
}

std::unordered_map<std::shared_ptr<Piece>, std::vector<sf::Vector2i>>& Global::Player::getValidMoves()
{
	validMoves.clear();
	getAllVaidMoves();
	// TODO: insert return statement here
	return this->validMoves;
}

bool Global::Player::KingMustMove()
{
	if (CheckPieces.size() > 1 && isCheck == true)
		return true;
	else
		return false;
}

bool Global::Player::isPiecePinned(std::shared_ptr<Piece> piece)
{
	for (auto pair : PinnedPieces)
	{
		if (pair.second == piece && piece->getPieceColor() == color)
			return true;
	}
	return false;
}

void Global::Player::CheckGhoda()
{
	// Define the possible movements for checking
	const int directions[8][2] = { { -1, -2 },{ -1, 2 },{ -2, -1 },{ -2, 1 },
		{ 1, -2 },{ 1, 2 },{ 2, -1 },{ 2, 1 } };

	for (const auto& dir : directions)
	{
		int newX = kingPosition.x + dir[0];
		int newY = kingPosition.y + dir[1];

		// Check if the new position is within the board boundaries
		if (newX >= 0 && newX <= 7 && newY >= 0 && newY <= 7)
		{
			// Check if the new position is occupied by an enemy piece (assuming method: isPositionOccupiedByEnemy)
			if (model.isPositionOccupiedByEnemy(sf::Vector2i(newX, newY), color))
			{
				auto pieceAtPosition = model.getPieceAtPosition(sf::Vector2i(newX, newY));

				// Check if the piece is a Ghoda (assuming method: getPieceType)
				if (pieceAtPosition->getPieceType() == Piece_Type::Ghoda)
				{
					isCheck = true;
					CheckPieces.push_back(std::make_pair(sf::Vector2i(newX, newY), pieceAtPosition));
				}
			}
		}
	}
}

void Global::Player::getAllVaidMoves()
{
	auto positionsOccupiedOnBoard = model.getPositioOccupiedOnBoard();
	for (auto position : positionsOccupiedOnBoard)
	{
		auto currentPiece = position.second;
		auto currentPosition = position.first;
		if (currentPiece->getPieceColor() == color)
		{
			std::vector<sf::Vector2i> currentPieceMovablePositions;
			auto PieceMovablePositions = model.GetPiecePositions(currentPiece, currentPosition);
			for (auto movablePosition : PieceMovablePositions)
			{
				model.getBoard()[currentPosition.x][currentPosition.y].unsetPiece();

				std::pair<sf::Vector2i, std::shared_ptr<Piece>> capturedPieceInfo;
				auto pair = std::make_pair(sf::Vector2i(-1, -1), std::make_shared<Piece>(Piece()));
				capturedPieceInfo = model.MovePiece(currentPiece, movablePosition, pair);

				if (currentPiece->getPieceType() == Global::Piece_Type::Raaja)
				{
					updateKingPosition(movablePosition);
				}

				// Check if the move results in the king being in check
				bool isKingInCheck = isInCheck();

				// Move the piece back to its original position
				model.MovePiece(currentPiece, currentPosition, capturedPieceInfo);

				if (currentPiece->getPieceType() == Global::Piece_Type::Raaja)
				{
					updateKingPosition(currentPosition);
				}
				if (movablePosition.x >= 0 && movablePosition.y >= 0 && movablePosition.x <= 7 && movablePosition.y <= 7)
					model.getBoard()[movablePosition.x][movablePosition.y].unsetPiece();

				if (capturedPieceInfo.second && capturedPieceInfo.second->getPieceType() != Global::Piece_Type::Null)
				{
					model.getBoard()[movablePosition.x][movablePosition.y].setPiece(capturedPieceInfo.second);
				}
				// If the king is not in check after the move, add the position to the list of valid moves
				if (!isKingInCheck)
				{
					currentPieceMovablePositions.push_back(movablePosition);
				}
			}
			validMoves.insert(std::make_pair(currentPiece, currentPieceMovablePositions));
		}
	}
}