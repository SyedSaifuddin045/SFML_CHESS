#include <Controller.h>
#include <iostream>

Controller::Controller(int height, int width, const sf::String& title)
	:view(sf::VideoMode(width, height), title)
{
	InitializeBoard();
}

void Controller::RunGame()
{
	view.Display(this->Board);
}

Controller::~Controller()
{
}

void Controller::InitializeBoard()
{
	float tileSize = 80.0f;

	if (!this->WhiteTexture.loadFromFile(RESOURCE "/textures/square/Light_Square.png"))
	{
		std::cout << "Failed to load White texture!" << std::endl;
	}

	//std::cout << "White texture size: " << WhiteTexture.getSize().x << "," << WhiteTexture.getSize().y << std::endl;

	if (!this->BlackTexture.loadFromFile(RESOURCE "/textures/square/Dark_Square.png"))
	{
		std::cout << "Failed to load Black texture!" << std::endl;
	}

	//std::cout << "Black texture size: " << BlackTexture.getSize().x << "," << BlackTexture.getSize().y << std::endl;

	float Xpos = 0.0f;
	float Ypos = 0.0f;
	for (int i = 0; i < rows; i++)
	{
		std::vector<Tile> rowTiles;
		for (int j = 0; j < cols; j++)
		{
			if ((i + j) % 2 == 0)
			{
				// White Tile
				Tile white(sf::Vector2f(tileSize, tileSize), WhiteTexture, sf::Vector2f(Xpos, Ypos));
				rowTiles.push_back(white);
			}
			else
			{
				// Black Tile
				Tile black(sf::Vector2f(tileSize, tileSize), BlackTexture, sf::Vector2f(Xpos, Ypos));
				rowTiles.push_back(black);
			}
			Xpos += tileSize;
		}
		this->Board.push_back(rowTiles);
		Xpos = 0.0f;
		Ypos += tileSize;
	}
}
