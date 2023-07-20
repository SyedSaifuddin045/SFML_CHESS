#include <Controller.h>
#include <Log.h>

Controller::Controller(int height, int width, const sf::String& title)
	:view(sf::VideoMode(width, height), title), model()
{
}
void Controller::TogglePiece(Piece& clickedPiece)
{
	if (clickedPiece.getPieceType() != Global::Piece_Type::Null)
	{
		if (!clickedPiece.isPieceSelected()) {
			clickedPiece.pieceToggleSelection();
		}
		else
		{
			// Deselect any previously selected piece
			for (auto& row : model.getBoard()) {
				for (auto& tile : row) {
					tile.getPiece().DeselectPiece();
				}
			}
			clickedPiece.pieceToggleSelection();
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
			Piece& clickedPiece = board[rowIndex][columnIndex].getPiece();
			TogglePiece(clickedPiece);
			const std::vector<sf::Vector2i>& positions = GetPiecePositions(clickedPiece, sf::Vector2i(rowIndex, columnIndex));
			for (auto position : positions)
			{
				if ((position.x + position.y) % 2 == 0)
					board[position.x][position.y].setHighlight(model.WhiteHighlightTexture);
				else
					board[position.x][position.y].setHighlight(model.BlackHighlightTexture);
			}
		}
	}
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

const std::vector<sf::Vector2i> Controller::GetPiecePositions(Piece& piece, sf::Vector2i boardPosition)
{
	std::vector<sf::Vector2i> availablePositions;

	Global::Color color = piece.getPieceColor();
	Global::Piece_Type p_type = piece.getPieceType();
	int forwardDirection = (color == Global::Color::white) ? -1 : 1;

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
		break;
	case Global::Piece_Type::Ghoda:
		break;
	case Global::Piece_Type::Unth:
		break;
	case Global::Piece_Type::Wazir:
		// Add rules for Wazir based on color
		break;
	case Global::Piece_Type::Raaja:
		// Add rules for Raaja based on color
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