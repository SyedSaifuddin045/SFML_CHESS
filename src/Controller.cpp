#include <Controller.h>
#include <Log.h>

Controller::Controller(int height, int width, const sf::String& title)
	:view(sf::VideoMode(width, height), title), model()
{
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
			sf::Vector2f worldMousePosition = window.mapPixelToCoords(mousePosition, view.getView());

			// Convert world coordinates to grid indices
			int columnIndex = static_cast<int>(worldMousePosition.x / model.tile_size);
			int rowIndex = static_cast<int>(worldMousePosition.y / model.tile_size);

			Piece clickedPiece = model.getBoard()[rowIndex][columnIndex].getPiece();
			std::cout << static_cast<int>(clickedPiece.getPieceType()) << std::endl;
			clickedPiece.setApplyShader(true);
		}
		window.clear();
		view.Display(model.getBoard());
		window.display();
	}
}

Controller::~Controller()
{
}