#include <GameModel.h>

std::pair<Global::Piece_Type, Global::Color> GameModel::GetPieceForPosiition(int row, int col)
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
			std::pair<Global::Piece_Type, Global::Color> p_Type_Color = GetPieceForPosiition(i, j);
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

void GameModel::MovePiece(std::shared_ptr<Piece> piece, sf::Vector2i position)
{
	auto find = positionsOccupiedOnBoard.find(position);
	// Removing if it is occupied by another piece (i.e., enemy)
	if (find != positionsOccupiedOnBoard.end())
	{
		std::cout << "Removed new position from map" << std::endl;
		positionsOccupiedOnBoard.erase(find);
	}

	// Removing the current position of the piece from the map using iterator
	for (auto it = positionsOccupiedOnBoard.begin(); it != positionsOccupiedOnBoard.end();)
	{
		if (it->second == piece)
		{
			std::cout << "Removed piece from map" << std::endl;
			it = positionsOccupiedOnBoard.erase(it); // This will return the next valid iterator after erasing the element.
			break;
		}
		else
		{
			++it; // Move to the next element
		}
	}

	// Setting piece to the new position tile
	std::cout << "Successfully set new piece at the position" << std::endl;
	Board[position.x][position.y].setPiece(piece);
	// Adding the new position for the piece
	std::cout << "Added the new Position with updated piece" << std::endl;
	positionsOccupiedOnBoard.insert(std::make_pair(position, piece));
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