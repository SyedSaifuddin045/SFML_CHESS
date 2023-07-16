#include <GameModel.h>

std::pair<Piece_Type, sf::String> GameModel::GetPieceForPosiition(int row, int col)
{
	if (row == 0)
	{
		if (col == 0 || col == 7)
			return std::make_pair(Piece_Type::Haanthi, "black");
		if (col == 1 || col == 6)
			return std::make_pair(Piece_Type::Ghoda, "black");
		if (col == 2 || col == 5)
			return std::make_pair(Piece_Type::Unth, "black");
		if(col == 3)
			return std::make_pair(Piece_Type::Wazir, "black");
		if(col == 4)
			return std::make_pair(Piece_Type::Raaja, "black");
	}
	if (row == 1)
		return std::make_pair(Piece_Type::Pyada, "black");
	if (row == 6)
		return std::make_pair(Piece_Type::Pyada, "white");
	if (row == 7)
	{
		if (col == 0 || col == 7)
			return std::make_pair(Piece_Type::Haanthi, "white");
		if (col == 1 || col == 6)
			return std::make_pair(Piece_Type::Ghoda, "white");
		if (col == 2 || col == 5)
			return std::make_pair(Piece_Type::Unth, "white");
		if (col == 3)
			return std::make_pair(Piece_Type::Wazir, "white");
		if (col == 4)
			return std::make_pair(Piece_Type::Raaja, "white");
	}

	return std::make_pair(Piece_Type::Null, sf::String());
}

void GameModel::InitializeBoard()
{

	if (!this->WhiteTexture->loadFromFile(RESOURCE "/textures/square/Light_Square.png"))
	{
		std::cout << "Failed to load White texture!" << std::endl;
	}

	if (!this->BlackTexture->loadFromFile(RESOURCE "/textures/square/Dark_Square.png"))
	{
		std::cout << "Failed to load Black texture!" << std::endl;
	}

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
				tile = Tile(sf::Vector2f(tile_size, tile_size), WhiteTexture, sf::Vector2f(Xpos, Ypos));
			}
			else
			{
				// Black Tile
				tile = Tile(sf::Vector2f(tile_size, tile_size), BlackTexture, sf::Vector2f(Xpos, Ypos));
			}
			std::pair<Piece_Type, sf::String> p_Type_Color = GetPieceForPosiition(i, j);
			tile.setPiece(PieceFactory::CreatePiece(p_Type_Color.first, p_Type_Color.second));
			rowTiles.push_back(tile);
			Xpos += tile_size;
		}
		this->Board.push_back(rowTiles);
		Xpos = 0.0f;
		Ypos += tile_size;
	}
}

GameModel::GameModel()
{
	WhiteTexture = std::make_shared<sf::Texture>();
	BlackTexture = std::make_shared<sf::Texture>();
	InitializeBoard();
}