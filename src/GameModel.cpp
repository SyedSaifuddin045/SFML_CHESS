#include <GameModel.h>

std::pair<Global::Piece_Type, Global::Color> GameModel::GetPieceTypeAndColorAt(int row, int col)
{
	if (row == 0)
	{
		if (col == 0 || col == 7)
			return std::make_pair(Global::Piece_Type::Haanthi, Global::GetColorFromString("black"));
		if (col == 1 || col == 6)
			return std::make_pair(Global::Piece_Type::Ghoda, Global::GetColorFromString("black"));
		if (col == 2 || col == 5)
			return std::make_pair(Global::Piece_Type::Unth, Global::GetColorFromString("black"));
		if (col == 3)
			return std::make_pair(Global::Piece_Type::Wazir, Global::GetColorFromString("black"));
		if (col == 4)
			return std::make_pair(Global::Piece_Type::Raaja, Global::GetColorFromString("black"));
	}
	if (row == 1)
		return std::make_pair(Global::Piece_Type::Pyada, Global::GetColorFromString("black"));
	if (row == 6)
		return std::make_pair(Global::Piece_Type::Pyada, Global::GetColorFromString("white"));
	if (row == 7)
	{
		if (col == 0 || col == 7)
			return std::make_pair(Global::Piece_Type::Haanthi, Global::GetColorFromString("white"));
		if (col == 1 || col == 6)
			return std::make_pair(Global::Piece_Type::Ghoda, Global::GetColorFromString("white"));
		if (col == 2 || col == 5)
			return std::make_pair(Global::Piece_Type::Unth, Global::GetColorFromString("white"));
		if (col == 3)
			return std::make_pair(Global::Piece_Type::Wazir, Global::GetColorFromString("white"));
		if (col == 4)
			return std::make_pair(Global::Piece_Type::Raaja, Global::GetColorFromString("white"));
	}

	return std::make_pair(Global::Piece_Type::Null, Global::GetColorFromString(""));
}

std::shared_ptr<Piece> GameModel::getPieceAtPosition(sf::Vector2i pos)
{
	auto found = positionsOccupiedOnBoard.find(pos);
	return (found != positionsOccupiedOnBoard.end()) ? found->second : nullptr;
}

void GameModel::InitializeBoard()
{
	float Xpos = 0.0f;
	float Ypos = 0.0f;
	for (int i = 0; i < rows; i++)
	{
		std::vector<Tile> rowTiles;
		for (int j = 0; j < cols; j++)
		{
			Tile tile;
			if ((i + j) % 2 == 0)
			{
				// White Tile
				tile = Tile(sf::Vector2f(tile_size, tile_size), WhiteTexture,WhiteHighlightTexture, sf::Vector2f(Xpos, Ypos), sf::Vector2i(i, j));
			}
			else
			{
				// Black Tile
				tile = Tile(sf::Vector2f(tile_size, tile_size), BlackTexture,BlackHighlightTexture, sf::Vector2f(Xpos, Ypos), sf::Vector2i(i, j));
			}
			std::pair<Global::Piece_Type, Global::Color> p_Type_Color = GetPieceTypeAndColorAt(i, j);
			Piece& currentPiece = PieceFactory::CreatePiece(p_Type_Color.first, p_Type_Color.second);
			if (currentPiece.getPieceType() != Global::Piece_Type::Null)
			{
				std::shared_ptr<Piece> currentPiecePointer = std::make_shared<Piece>(currentPiece);
				tile.setPiece(currentPiecePointer);
				this->positionsOccupiedOnBoard.insert(std::make_pair(sf::Vector2i(i,j),currentPiecePointer));
			}
			rowTiles.push_back(tile);
			Xpos += tile_size;
		}
		this->Board.push_back(rowTiles);
		Xpos = 0.0f;
		Ypos += tile_size;
	}
}

bool GameModel::isOccupied(sf::Vector2i position)
{
	auto foundPosition = positionsOccupiedOnBoard.find(position);
	if (foundPosition != positionsOccupiedOnBoard.end())
	{
		return true;
	}
	return false;
}

bool GameModel::isPositionOccupiedByEnemy(sf::Vector2i position,Global::Color ourColor)
{
	Global::Color enemyColor = (ourColor == Global::Color::white) ? Global::Color::black : Global::Color::white;

	auto foundPosition = positionsOccupiedOnBoard.find(position);
	if (foundPosition != positionsOccupiedOnBoard.end())
	{
		Global::Color pieceColor = foundPosition->second->getPieceColor();
		if (pieceColor == enemyColor)
			return true;
	}
	//position not occupied by enemy
	return false;
}

std::pair<sf::Vector2i, std::shared_ptr<Piece>> GameModel::MovePiece(std::shared_ptr<Piece> piece, sf::Vector2i position, std::pair<sf::Vector2i, std::shared_ptr<Piece>> capturePair)
{
	auto find = positionsOccupiedOnBoard.find(position);
	std::pair<sf::Vector2i, std::shared_ptr<Piece>> pair;
	// Removing if it is occupied by another piece (i.e., enemy)
	if (find != positionsOccupiedOnBoard.end())
	{
		pair = *find;
		positionsOccupiedOnBoard.erase(find);
	}

	// Removing the current position of the piece from the map using iterator
	for (auto it = positionsOccupiedOnBoard.begin(); it != positionsOccupiedOnBoard.end();)
	{
		if (it->second == piece)
		{
			//std::cout << "Removed piece from map" << std::endl;
			it = positionsOccupiedOnBoard.erase(it); // This will return the next valid iterator after erasing the element.
			break;
		}
		else
		{
			++it; // Move to the next element
		}
	}

	// Setting piece to the new position tile
	bool setSuccess = false;
	if (position.x >= 0 && position.y >= 0 && position.x <= 7 && position.y <= 7)
	{
		setSuccess = true;
		Board[position.x][position.y].setPiece(piece);
	}
	// Adding the new position for the piece
	if (setSuccess)
	{
		positionsOccupiedOnBoard.insert(std::make_pair(position, piece));
	}
	if (capturePair.second)
	{
		if (capturePair.second->getPieceType() != Global::Piece_Type::Null)
		{
			positionsOccupiedOnBoard.insert(capturePair);
			Board[capturePair.first.x][capturePair.first.y].setPiece(capturePair.second);
		}
	}
	return pair;
}

std::unordered_map<sf::Vector2i, std::shared_ptr<Piece>>&  GameModel::getPositioOccupiedOnBoard()
{
	return this->positionsOccupiedOnBoard;
}

void GameModel::executeCommand(std::unique_ptr<Command> command)
{
	command->execute();
	executedCommands.push(std::move(command));
}

void GameModel::undoLastCommand() {
	if (!executedCommands.empty()) {
		std::unique_ptr<Command> lastCommand = std::move(executedCommands.top());
		executedCommands.pop();
		lastCommand->undo();
		undoneCommands.push(std::move(lastCommand));
	}
}

void GameModel::redoCommand()
{
	if (!undoneCommands.empty())
	{
		std::unique_ptr<Command> commandToRedo = std::move(undoneCommands.top());
		undoneCommands.pop();

		commandToRedo->execute();
		executedCommands.push(std::move(commandToRedo));
	}
}


GameModel::GameModel()
{
	WhiteTexture = std::make_shared<sf::Texture>();
	BlackTexture = std::make_shared<sf::Texture>();

	WhiteHighlightTexture = std::make_shared<sf::Texture>();
	BlackHighlightTexture = std::make_shared<sf::Texture>();

	if (!this->WhiteTexture->loadFromFile(RESOURCE "/textures/square/Light_Square.png"))
	{
		std::cout << "Failed to load White texture!" << std::endl;
	}

	if (!this->BlackTexture->loadFromFile(RESOURCE "/textures/square/Dark_Square.png"))
	{
		std::cout << "Failed to load Black texture!" << std::endl;
	}

	if (!this->WhiteHighlightTexture->loadFromFile(RESOURCE "/textures/square/Light_Square_Highlight.png"))
	{
		std::cout << "Failed to load White texture!" << std::endl;
	}

	if (!this->BlackHighlightTexture->loadFromFile(RESOURCE "/textures/square/Dark_Square_Highlight.png"))
	{
		std::cout << "Failed to load Black texture!" << std::endl;
	}
	InitializeBoard();
}


void GameModel::Reset()
{
	Board.clear();
	positionsOccupiedOnBoard.clear();
	while (!executedCommands.empty())
	{
		executedCommands.pop();
	}
	while (!undoneCommands.empty())
	{
		undoneCommands.pop();
	}
	InitializeBoard();
}

const std::vector<sf::Vector2i> GameModel::GetPiecePositions(std::shared_ptr<Piece> piece, sf::Vector2i boardPosition)
{
	std::vector<sf::Vector2i> availablePositions;

	Global::Color color = piece->getPieceColor();
	Global::Piece_Type p_type = piece->getPieceType();
	int forwardDirection = (color == Global::Color::white) ? -1 : 1;

	int vertical = 0;
	int horizontal = 0;

	switch (p_type)
	{
	case Global::Piece_Type::Pyada:
	{
		if ((boardPosition.x == 1 || boardPosition.x == 6) && !isOccupied(sf::Vector2i(boardPosition.x + (1 * forwardDirection), boardPosition.y)) && !isOccupied(sf::Vector2i(boardPosition.x + (2 * forwardDirection), boardPosition.y)))
		{
			availablePositions.push_back(sf::Vector2i(boardPosition.x + (2 * forwardDirection), boardPosition.y));
		}
		if (!isOccupied(sf::Vector2i(boardPosition.x + (1 * forwardDirection), boardPosition.y)))
		{
			availablePositions.push_back(sf::Vector2i(boardPosition.x + (1 * forwardDirection), boardPosition.y));
		}
		if (boardPosition.y != 0 && isPositionOccupiedByEnemy(sf::Vector2i(boardPosition.x + (1 * forwardDirection), boardPosition.y - 1), color))
			availablePositions.push_back(sf::Vector2i(boardPosition.x + (1 * forwardDirection), boardPosition.y - 1));
		if (boardPosition.y != 7 && isPositionOccupiedByEnemy(sf::Vector2i(boardPosition.x + (1 * forwardDirection), boardPosition.y + 1), color))
			availablePositions.push_back(sf::Vector2i(boardPosition.x + (1 * forwardDirection), boardPosition.y + 1));
		break;
	}
	case Global::Piece_Type::Haanthi:
		for (int j = 1; j <= 2; j++)
		{
			if (j == 1)
			{
				vertical = 1, horizontal = 1;
			}
			else {
				vertical = -1, horizontal = -1;
			}
			//For Vertical
			for (int i = boardPosition.y; i >= 0 && i <= 7;)
			{
				i += vertical;
				if (!isOccupied(sf::Vector2i(boardPosition.x, i)) || isPositionOccupiedByEnemy(sf::Vector2i(boardPosition.x, i), color))
				{
					availablePositions.push_back(sf::Vector2i(boardPosition.x, i));
					if (isPositionOccupiedByEnemy(sf::Vector2i(boardPosition.x, i), color))
						break;
				}
				else
				{
					break;
				}
			}
			//For Horizontal
			for (int i = boardPosition.x; i >= 0 && i <= 7;)
			{
				i += horizontal;
				if (!isOccupied(sf::Vector2i(i, boardPosition.y)) || isPositionOccupiedByEnemy(sf::Vector2i(i, boardPosition.y), color))
				{
					availablePositions.push_back(sf::Vector2i(i, boardPosition.y));
					if (isPositionOccupiedByEnemy(sf::Vector2i(i, boardPosition.y), color))
						break;
				}
				else
				{
					break;
				}
			}
		}
		break;
	case Global::Piece_Type::Ghoda:
		for (int i = 1; i <= 4; i++)
		{
			if (i % 2 == 0)
			{
				vertical = (i == 2) ? 1 : -1;
				if (boardPosition.x + (2 * vertical) <= 7 && boardPosition.x + (2 * vertical) >= 0)
				{
					if (boardPosition.y + 1 <= 7 && boardPosition.y + 1 >= 0)
					{
						if (!isOccupied(sf::Vector2i(boardPosition.x + (2 * vertical), boardPosition.y + 1)) || isPositionOccupiedByEnemy(sf::Vector2i(boardPosition.x + (2 * vertical), boardPosition.y + 1), color))
							availablePositions.push_back(sf::Vector2i(boardPosition.x + (2 * vertical), boardPosition.y + 1));
					}
					if (boardPosition.y - 1 <= 7 && boardPosition.y - 1 >= 0)
					{
						if (!isOccupied(sf::Vector2i(boardPosition.x + (2 * vertical), boardPosition.y - 1)) || isPositionOccupiedByEnemy(sf::Vector2i(boardPosition.x + (2 * vertical), boardPosition.y - 1), color))
							availablePositions.push_back(sf::Vector2i(boardPosition.x + (2 * vertical), boardPosition.y - 1));
					}
				}
			}
			else
			{
				horizontal = (i == 1) ? 1 : -1;
				if (boardPosition.y + (2 * horizontal) <= 7 && boardPosition.y + (2 * horizontal) >= 0)
				{
					if (boardPosition.x + 1 <= 7 && boardPosition.x + 1 >= 0)
					{
						if (!isOccupied(sf::Vector2i(boardPosition.x + 1, boardPosition.y + (2 * horizontal))) || isPositionOccupiedByEnemy(sf::Vector2i(boardPosition.x + 1, boardPosition.y + (2 * horizontal)), color))
							availablePositions.push_back(sf::Vector2i(boardPosition.x + 1, boardPosition.y + (2 * horizontal)));
					}
					if (boardPosition.x - 1 <= 7 && boardPosition.x - 1 >= 0)
					{
						if (!isOccupied(sf::Vector2i(boardPosition.x - 1, boardPosition.y + (2 * horizontal))) || isPositionOccupiedByEnemy(sf::Vector2i(boardPosition.x - 1, boardPosition.y + (2 * horizontal)), color))
							availablePositions.push_back(sf::Vector2i(boardPosition.x - 1, boardPosition.y + (2 * horizontal)));
					}
				}
			}
			horizontal, vertical = 0;
		}
		break;
	case Global::Piece_Type::Unth:
		for (int i = 1; i <= 4; i++)
		{
			vertical = (i == 2 || i == 4) ? 1 : -1;
			horizontal = (i == 1 || i == 4) ? 1 : -1;
			int newRow = boardPosition.x + vertical;
			int newCol = boardPosition.y + horizontal;

			while (newRow >= 0 && newRow < 8 && newCol >= 0 && newCol < 8)
			{
				if (!isOccupied(sf::Vector2i(newRow, newCol)) || isPositionOccupiedByEnemy(sf::Vector2i(newRow, newCol), color))
					availablePositions.push_back(sf::Vector2i(newRow, newCol));

				if (isPositionOccupiedByEnemy(sf::Vector2i(newRow, newCol), color) || isOccupied(sf::Vector2i(newRow, newCol)))
					break;

				newRow += vertical;
				newCol += horizontal;
			}
		}

		break;
	case Global::Piece_Type::Wazir:
		for (int i = 1; i <= 8; i++)
		{
			vertical = (i == 2 || i == 4) ? 1 : -1;
			horizontal = (i == 1 || i == 4) ? 1 : -1;

			int newRow = boardPosition.x + vertical;
			int newCol = boardPosition.y + horizontal;

			while (newRow >= 0 && newRow < 8 && newCol >= 0 && newCol < 8)
			{
				if (!isOccupied(sf::Vector2i(newRow, newCol)) || isPositionOccupiedByEnemy(sf::Vector2i(newRow, newCol), color))
					availablePositions.push_back(sf::Vector2i(newRow, newCol));

				if (isPositionOccupiedByEnemy(sf::Vector2i(newRow, newCol), color) || isOccupied(sf::Vector2i(newRow, newCol)))
					break;

				newRow += vertical;
				newCol += horizontal;
			}
		}

		for (int j = 1; j <= 2; j++)
		{
			if (j == 1)
			{
				vertical = 1, horizontal = 1;
			}
			else {
				vertical = -1, horizontal = -1;
			}
			//For Vertical
			for (int i = boardPosition.y; i >= 0 && i <= 7;)
			{
				i += vertical;
				if (!isOccupied(sf::Vector2i(boardPosition.x, i)) || isPositionOccupiedByEnemy(sf::Vector2i(boardPosition.x, i), color))
				{
					availablePositions.push_back(sf::Vector2i(boardPosition.x, i));
					if (isPositionOccupiedByEnemy(sf::Vector2i(boardPosition.x, i), color))
						break;
				}
				else
				{
					break;
				}
			}
			//For Horizontal
			for (int i = boardPosition.x; i >= 0 && i <= 7;)
			{
				i += horizontal;
				if (!isOccupied(sf::Vector2i(i, boardPosition.y)) || isPositionOccupiedByEnemy(sf::Vector2i(i, boardPosition.y), color))
				{
					availablePositions.push_back(sf::Vector2i(i, boardPosition.y));
					if (isPositionOccupiedByEnemy(sf::Vector2i(i, boardPosition.y), color))
						break;
				}
				else
				{
					break;
				}
			}
		}
		break;
	case Global::Piece_Type::Raaja:
		for (int i = -1; i <= 1; i++)
		{
			for (int j = -1; j <= 1; j++)
			{
				if (i == 0 && j == 0)
					continue;

				int newRow = boardPosition.x + i;
				int newCol = boardPosition.y + j;

				if (newRow >= 0 && newRow < 8 && newCol >= 0 && newCol < 8)
				{
					if (!isOccupied(sf::Vector2i(newRow, newCol)) || isPositionOccupiedByEnemy(sf::Vector2i(newRow, newCol), color))
						availablePositions.push_back(sf::Vector2i(newRow, newCol));
				}
			}
		}
		break;
	case Global::Piece_Type::Null:
	default:
		break;
	}

	return availablePositions;
}
