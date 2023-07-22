#include <Controller.h>
#include <Log.h>

Controller::Controller(int height, int width, const sf::String& title)
	:view(sf::VideoMode(width, height), title), model()
{
	lastClickedPiece = std::make_shared<Piece>();
	lastMouseClickPosition = sf::Vector2i(-1, -1);
}

void Controller::TogglePiece(std::shared_ptr<Piece> clickedPiece, sf::Vector2i pos)
{
	if (clickedPiece->getPieceType() != Global::Piece_Type::Null)
	{
		if (!clickedPiece->isPieceSelected()) {
			clickedPiece->pieceToggleSelection();
			lastClickedPiece = clickedPiece;

			// Get available move positions for the selected piece and highlight the tiles
			ClickedPiecePositions = GetPiecePositions(clickedPiece, pos);
			for (auto position : ClickedPiecePositions)
			{
				if (position.x <= 7 && position.x >= 0 && position.y <= 7 && position.y >= 0)
				{
					if ((position.x + position.y) % 2 == 0)
						model.getBoard()[position.x][position.y].setHighlightTexture();
					else
						model.getBoard()[position.x][position.y].setHighlightTexture();
				}
			}
		}
	}
}


void Controller::HandleInput(sf::RenderWindow& window)
{
	if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
	{
		sf::Vector2i mousePosition = sf::Mouse::getPosition(window);
		sf::Vector2f worldMousePosition = this->view.getWindow().mapPixelToCoords(mousePosition, view.getView());

		// Convert world coordinates to grid indices
		int columnIndex = static_cast<int>(worldMousePosition.x / model.tile_size);
		int rowIndex = static_cast<int>(worldMousePosition.y / model.tile_size);

		std::vector<std::vector<Tile>>& board = model.getBoard();
		if (rowIndex >= 0 && rowIndex < board.size() && columnIndex >= 0 && columnIndex < board[rowIndex].size())
		{

			bool retFlag;
			PreventDuplicateClicks(rowIndex, columnIndex, retFlag);
			if (retFlag) return;

			Tile& clickedTile = board[rowIndex][columnIndex];
			// Select the clicked piece if any
			if (lastClickedPiece)
			{
				for (auto position : ClickedPiecePositions)
				{
					if (position == clickedTile.getGamePosition())
					{
						lastClickedTile->unsetPiece();
						model.MovePiece(lastClickedPiece, clickedTile.getGamePosition());
					}
				}
				ResetTiles();
				lastClickedPiece.reset();
			}
			std::shared_ptr<Piece> clickedPiece = clickedTile.getPiece();
			if (clickedPiece)
			{
				lastClickedTile = &clickedTile;
				if (clickedPiece == lastClickedPiece)
					return;
				TogglePiece(clickedPiece, sf::Vector2i(rowIndex, columnIndex));
			}

		}
	}
}

void Controller::ResetTiles()
{
	lastClickedPiece->DeselectPiece();
	lastClickedPiece = nullptr;
	for (auto& row : model.getBoard()) {
		for (auto& tile : row) {
			tile.setNormalTexture();
		}
	}
}

void Controller::PreventDuplicateClicks(int rowIndex, int columnIndex, bool& retFlag)
{
	retFlag = true;
	if (sf::Vector2i(rowIndex, columnIndex) == lastMouseClickPosition) {
		return; // Skip further processing for this mouse click
	}
	lastMouseClickPosition = sf::Vector2i(rowIndex, columnIndex);
	retFlag = false;
}


void Controller::RunGame()
{
	sf::RenderWindow& window = view.getWindow();
	window.setView(view.getView());

	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
			break;
			if (event.type == sf::Event::Resized)
				view.ReSizeView();
			break;
		}
		HandleInput(window);
		window.clear();
		view.Display(model.getBoard());
		window.display();
	}
}

const std::vector<sf::Vector2i> Controller::GetPiecePositions(std::shared_ptr<Piece> piece, sf::Vector2i boardPosition)
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
		if ((boardPosition.x == 1 || boardPosition.x == 6) && !model.isOccupied(sf::Vector2i(boardPosition.x + (1 * forwardDirection), boardPosition.y)) && !model.isOccupied(sf::Vector2i(boardPosition.x + (2 * forwardDirection), boardPosition.y)))
		{
			availablePositions.push_back(sf::Vector2i(boardPosition.x + (2 * forwardDirection), boardPosition.y));
		}
		if (!model.isOccupied(sf::Vector2i(boardPosition.x + (1 * forwardDirection), boardPosition.y)))
		{
			availablePositions.push_back(sf::Vector2i(boardPosition.x + (1 * forwardDirection), boardPosition.y));
		}
		if (boardPosition.y != 0 && model.isPositionOccupiedByEnemy(sf::Vector2i(boardPosition.x + (1 * forwardDirection), boardPosition.y - 1), color))
			availablePositions.push_back(sf::Vector2i(boardPosition.x + (1 * forwardDirection), boardPosition.y - 1));
		if (boardPosition.y != 7 && model.isPositionOccupiedByEnemy(sf::Vector2i(boardPosition.x + (1 * forwardDirection), boardPosition.y + 1), color))
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
				if (!model.isOccupied(sf::Vector2i(boardPosition.x,i)) || model.isPositionOccupiedByEnemy(sf::Vector2i(boardPosition.x,i), color))
				{
					availablePositions.push_back(sf::Vector2i(boardPosition.x,i));
					if (model.isPositionOccupiedByEnemy(sf::Vector2i(boardPosition.x,i), color))
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
				if (!model.isOccupied(sf::Vector2i(i, boardPosition.y)) || model.isPositionOccupiedByEnemy(sf::Vector2i(i, boardPosition.y), color))
				{
					availablePositions.push_back(sf::Vector2i(i, boardPosition.y));
					if (model.isPositionOccupiedByEnemy(sf::Vector2i(i, boardPosition.y), color))
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
						if (!model.isOccupied(sf::Vector2i(boardPosition.x + (2 * vertical), boardPosition.y + 1)) || model.isPositionOccupiedByEnemy(sf::Vector2i(boardPosition.x + (2 * vertical), boardPosition.y + 1), color))
							availablePositions.push_back(sf::Vector2i(boardPosition.x + (2 * vertical), boardPosition.y + 1));
					}
					if (boardPosition.y - 1 <= 7 && boardPosition.y - 1 >= 0)
					{
						if (!model.isOccupied(sf::Vector2i(boardPosition.x + (2 * vertical), boardPosition.y - 1)) || model.isPositionOccupiedByEnemy(sf::Vector2i(boardPosition.x + (2 * vertical), boardPosition.y - 1), color))
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
						if (!model.isOccupied(sf::Vector2i(boardPosition.x + 1, boardPosition.y + (2 * horizontal))) || model.isPositionOccupiedByEnemy(sf::Vector2i(boardPosition.x + 1, boardPosition.y + (2 * horizontal)), color))
							availablePositions.push_back(sf::Vector2i(boardPosition.x + 1, boardPosition.y + (2 * horizontal)));
					}
					if (boardPosition.x - 1 <= 7 && boardPosition.x - 1 >= 0)
					{
						if (!model.isOccupied(sf::Vector2i(boardPosition.x - 1, boardPosition.y + (2 * horizontal))) || model.isPositionOccupiedByEnemy(sf::Vector2i(boardPosition.x - 1, boardPosition.y + (2 * horizontal)), color))
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
				if (!model.isOccupied(sf::Vector2i(newRow, newCol)) || model.isPositionOccupiedByEnemy(sf::Vector2i(newRow, newCol), color))
					availablePositions.push_back(sf::Vector2i(newRow, newCol));

				if (model.isPositionOccupiedByEnemy(sf::Vector2i(newRow, newCol), color) || model.isOccupied(sf::Vector2i(newRow,newCol)))
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
				if (!model.isOccupied(sf::Vector2i(newRow, newCol)) || model.isPositionOccupiedByEnemy(sf::Vector2i(newRow, newCol), color))
					availablePositions.push_back(sf::Vector2i(newRow, newCol));

				if (model.isPositionOccupiedByEnemy(sf::Vector2i(newRow, newCol), color) || model.isOccupied(sf::Vector2i(newRow, newCol)))
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
				if (!model.isOccupied(sf::Vector2i(boardPosition.x, i)) || model.isPositionOccupiedByEnemy(sf::Vector2i(boardPosition.x, i), color))
				{
					availablePositions.push_back(sf::Vector2i(boardPosition.x, i));
					if (model.isPositionOccupiedByEnemy(sf::Vector2i(boardPosition.x, i), color))
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
				if (!model.isOccupied(sf::Vector2i(i, boardPosition.y)) || model.isPositionOccupiedByEnemy(sf::Vector2i(i, boardPosition.y), color))
				{
					availablePositions.push_back(sf::Vector2i(i, boardPosition.y));
					if (model.isPositionOccupiedByEnemy(sf::Vector2i(i, boardPosition.y), color))
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
					continue; // Skip the current position (King's own position)

				int newRow = boardPosition.x + i;
				int newCol = boardPosition.y + j;

				if (newRow >= 0 && newRow < 8 && newCol >= 0 && newCol < 8)
				{
					if (!model.isOccupied(sf::Vector2i(newRow, newCol)) || model.isPositionOccupiedByEnemy(sf::Vector2i(newRow, newCol), color))
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

Controller::~Controller()
{
}