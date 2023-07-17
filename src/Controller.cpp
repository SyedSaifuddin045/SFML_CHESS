#include <Controller.h>
#include <Log.h>

Controller::Controller(int height, int width, const sf::String& title)
	:view(sf::VideoMode(width, height), title), model()
{
}
void Controller::TogglePieceAt(sf::Vector2i mousePosition)
{
	sf::Vector2f worldMousePosition = this->view.getWindow().mapPixelToCoords(mousePosition, view.getView());

	// Convert world coordinates to grid indices
	int columnIndex = static_cast<int>(worldMousePosition.x / model.tile_size);
	int rowIndex = static_cast<int>(worldMousePosition.y / model.tile_size);

	//std::cout << "Toggle Selection" << std::endl;
	Piece& clickedPiece = model.getBoard()[rowIndex][columnIndex].getPiece();
	// Toggle the piece selection state only if it's not already selected
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
		if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
		{
			sf::Vector2i mousePosition = sf::Mouse::getPosition(window);
			TogglePieceAt(mousePosition);
		}
		window.clear();
		view.Display(model.getBoard());
		window.display();
	}
}

Controller::~Controller()
{
}