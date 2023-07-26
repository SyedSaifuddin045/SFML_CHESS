#include <Controller.h>
#include <Log.h>

Controller::Controller(int height, int width, const sf::String& title)
	:view(sf::VideoMode(width, height), title), model(), WhitePlayer(sf::Vector2i(7, 4), model), BlackPlayer(sf::Vector2i(0, 4), model)
{
	lastClickedPiece = std::make_shared<Piece>();
	lastMouseClickPosition = sf::Vector2i(-1, -1);
	currentPlayer = &WhitePlayer;
}

void Controller::TogglePiece(std::shared_ptr<Piece> clickedPiece, sf::Vector2i pos)
{
	if (clickedPiece->getPieceType() != Global::Piece_Type::Null)
	{
		clickedPiece->pieceToggleSelection();
		lastClickedPiece = clickedPiece;

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
						//model.MovePiece(lastClickedPiece, clickedTile.getGamePosition());
						if (lastClickedPiece->getPieceType() == Global::Piece_Type::Raaja)
						{
							currentPlayer->updateKingPosition(clickedTile.getGamePosition());
						}
						std::unique_ptr<Command> moveCommand = std::make_unique<MoveCommand>(model, lastClickedPiece, clickedTile.getGamePosition(), lastClickedTile->getGamePosition());
						model.executeCommand(std::move(moveCommand));
						ChangePlayer();
					}
				}
				ResetTiles();
				lastClickedPiece.reset();
			}
			std::shared_ptr<Piece> clickedPiece = clickedTile.getPiece();
			if (clickedPiece)
			{
				if (clickedPiece->getPieceColor() != currentPlayer->getColor())
					return;
				bool isKingCheck = currentPlayer->isInCheck();
				if (currentPlayer->KingMustMove() && clickedPiece->getPieceType() != Global::Piece_Type::Raaja)
					return;

				lastClickedTile = &clickedTile;

				if (isKingCheck || currentPlayer->isPiecePinned(clickedPiece) )
				{
					auto validMoves = currentPlayer->getValidMoves();
					auto find = validMoves.find(clickedPiece);
					if (find != validMoves.end())
						ClickedPiecePositions = find->second;
				}
				else
				{
					ClickedPiecePositions = model.GetPiecePositions(clickedPiece, sf::Vector2i(rowIndex, columnIndex));
				}
				TogglePiece(clickedPiece, sf::Vector2i(rowIndex, columnIndex));
			}

		}
	}
}

void Controller::ChangePlayer()
{
	if (currentPlayer->getColor() == Global::Color::white)
		currentPlayer = &BlackPlayer;
	else
		currentPlayer = &WhitePlayer;

	currentPlayer->isInCheck();
}

void Controller::ResetTiles()
{
	if (lastClickedPiece)
	{
		lastClickedPiece->DeselectPiece();
	}
	lastClickedPiece = nullptr;
	for (auto& row : model.getBoard()) {
		for (auto& tile : row) {
			tile.isHighlighted = false;
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

	bool undoKeyPressed = false;
	bool redoKeyPressed = false;

	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();

			if (event.type == sf::Event::Resized)
				view.ReSizeView();
		}

		// Check for key presses and set corresponding flags
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Z) && !undoKeyPressed)
		{
			std::cout << "Pressed Undo key" << std::endl;
			model.undoLastCommand();
			ChangePlayer();
			undoKeyPressed = true;
		}
		else if (!sf::Keyboard::isKeyPressed(sf::Keyboard::Z))
		{
			undoKeyPressed = false;
		}

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::R) && !redoKeyPressed)
		{
			std::cout << "Pressed Redo key" << std::endl;
			model.redoCommand();
			ChangePlayer();
			redoKeyPressed = true;
		}
		else if (!sf::Keyboard::isKeyPressed(sf::Keyboard::R))
		{
			redoKeyPressed = false;
		}

		HandleInput(window);
		window.clear();
		view.Display(model.getBoard());
		window.display();
	}
}

Controller::~Controller()
{
}