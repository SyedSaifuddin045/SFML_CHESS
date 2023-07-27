#include <Controller.h>
#include <Log.h>

Controller::Controller(int height, int width, const sf::String& title)
	:view(sf::VideoMode(width, height), title), model(), WhitePlayer(sf::Vector2i(7, 4), model), BlackPlayer(sf::Vector2i(0, 4), model)
{
	lastClickedPiece = std::make_shared<Piece>();
	lastMouseClickPosition = sf::Vector2i(-1, -1);
	currentPlayer = &WhitePlayer;
	isPaused = false;
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

				if (isKingCheck || currentPlayer->isPiecePinned(clickedPiece))
				{
					auto validMoves = currentPlayer->getValidMoves();
					if (validMoves.size() <= 0)
					{
						currentPlayer->checkMate();
					}
					auto find = validMoves.find(clickedPiece);
					if (find != validMoves.end())
						ClickedPiecePositions = find->second;
					else
						return;
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
	window.setFramerateLimit(30);
	window.setView(view.getView());

	ImGui::SFML::Init(window);

	ImGuiStyle& style = ImGui::GetStyle();
	style.FrameRounding = 5.0f;

	bool undoKeyPressed = false;
	bool redoKeyPressed = false;

	sf::Clock clock;
	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			ImGui::SFML::ProcessEvent(event);
			if (event.type == sf::Event::Closed)
				window.close();

			if (event.type == sf::Event::Resized)
				view.ReSizeView();
		}
		ImGui::SFML::Update(window, clock.restart());
		ImGuiWindowFlags flags = ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoBackground;

		ImGui::SetNextWindowPos(ImVec2(0, 0), ImGuiCond_Always);

		ImGui::Begin("Pause/Resume", 0, flags);
		if (isPaused)
		{
			if (ImGui::Button("PLAY"))
			{
				isPaused = false;
			}
		}
		else
		{
			if (ImGui::Button("PAUSE"))
			{
				isPaused = true;
			}
		}
		ImGui::End();
		if (isPaused)
		{
			ImVec2 windowPos(window.getSize().x / 2.0f, window.getSize().y / 2.0f);
			ImVec2 windowSize(window.getSize().x, 20);
			ImGui::SetNextWindowPos(ImVec2(windowPos.x - windowSize.x / 2, windowPos.y - windowSize.y / 2), ImGuiCond_Always);
			ImGui::SetNextWindowSize(windowSize, ImGuiCond_Always);
			ImGui::Begin("Game Paused", nullptr, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoTitleBar);
			ImVec2 textSize = ImGui::CalcTextSize("The Game is Paused");
			ImGui::SetCursorPosX((ImGui::GetWindowContentRegionMax().x + ImGui::GetWindowContentRegionMin().x - textSize.x) * 0.5f);
			ImGui::SetCursorPosY((ImGui::GetWindowContentRegionMax().y + ImGui::GetWindowContentRegionMin().y - textSize.y) * 0.5f);
			ImGui::Text("The Game is Paused");
			ImGui::End();
		}
		if (!isPaused)
		{
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

			ImGuiWindowFlags flags = ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoBackground;

			ImGui::SetNextWindowPos(ImVec2(window.getSize().x - 100, 0), ImGuiCond_Always);

			ImGui::Begin("Buttons", 0, flags);

			if (ImGui::Button("UNDO"))
			{
				model.undoLastCommand();
				ChangePlayer();
			}
			ImGui::SameLine();
			if (ImGui::Button("REDO"))
			{
				model.redoCommand();
				ChangePlayer();
			}

			//ImGui::ShowDemoWindow();
			ImGui::End();

			HandleInput(window);
		}
		window.clear();
		view.Display(model.getBoard());
		ImGui::SFML::Render(window);
		window.display();
	}
	ImGui::SFML::Shutdown();
}

Controller::~Controller()
{
}