#include <Controller.h>
#include <Log.h>

Controller::Controller(int height, int width, const sf::String& title)
	:view(sf::VideoMode(width, height), title), model(), WhitePlayer(sf::Vector2i(7, 4), model), BlackPlayer(sf::Vector2i(0, 4), model)
{
	SetUpController();
}

void Controller::SetUpController()
{
	lastClickedPiece = std::make_shared<Piece>();
	lastMouseClickPosition = sf::Vector2i(-1, -1);
	currentPlayer = &WhitePlayer;
	isPaused = false;
	checkMate = false;
	gameState = GameState::MainMenu;
	playOnline = false;
	server = nullptr;
	client = nullptr;
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


void Controller::HandleInput(sf::RenderWindow& window, bool isKingCheck)
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
						std::unique_ptr<Command> moveCommand = std::make_unique<MoveCommand>(model, model.getPieceAtPosition(lastClickedTile->getGamePosition()), clickedTile.getGamePosition(), lastClickedTile->getGamePosition());
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
				if (currentPlayer->KingMustMove() && clickedPiece->getPieceType() != Global::Piece_Type::Raaja)
					return;

				lastClickedTile = &clickedTile;

				if (isKingCheck || currentPlayer->isPiecePinned(clickedPiece) || clickedPiece->getPieceType() == Global::Piece_Type::Raaja)
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

void Controller::CreateServer(unsigned short port)
{
	if (!server)
	{
		server = new Server(model, port);
		server->start();
	}
}

void Controller::CreateClient(const char* address, unsigned short port)
{
	if (!client)
	{
		client = new Client(model);
		client->Connect(address, port);
	}
}

void Controller::WaitForConnection()
{
	if (server)
	{
		ShowSpinner();
		if (server->IsClientConnected())
		{
			std::cout << "Client Successfully Connected To Server!" << std::endl;
			gameState = GameState::PlayGame;
		}
	}
	if (client)
	{
		if (client->isConnectedToServer())
		{
			std::cout << "Client Successfully Connected To Server!" << std::endl;
			gameState = GameState::PlayGame;
		}
	}
}

void Controller::ShowSpinner()
{
	static bool use_work_area = true;
	static ImGuiWindowFlags flags = ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoSavedSettings | ImGuiConfigFlags_NavEnableKeyboard;

	const ImGuiViewport* viewport = ImGui::GetMainViewport();
	ImGui::SetNextWindowPos(use_work_area ? viewport->WorkPos : viewport->Pos);
	ImGui::SetNextWindowSize(use_work_area ? viewport->WorkSize : viewport->Size);

	if (ImGui::Begin("Fullscreen window", nullptr, flags))
	{
		float radius = 30.0f; // Distance from the center of rotation
		float windowWidth = ImGui::GetWindowWidth();
		float windowCenterX = ImGui::GetWindowPos().x + windowWidth * 0.5f;

		ImGui::SetCursorPos(ImVec2(windowCenterX - ImGui::CalcTextSize("WAITING FOR CONNECTION...").x * 0.5f, ImGui::GetWindowHeight() / 2.0 - 50.0f));
		ImGui::Text("WAITING FOR CONNECTION...");

		float angle = ImGui::GetTime() * 2.0f * 3.1415926f; // 2.0f to control rotation speed

		// Calculate the positions of p1 and p2 using trigonometry
		ImVec2 center(windowCenterX, ImGui::GetWindowHeight() / 2.0f);
		ImVec2 p1(center.x - radius * cosf(angle), center.y - radius * sinf(angle));
		ImVec2 p2(center.x + radius * cosf(angle), center.y + radius * sinf(angle));

		// Draw the rotating line
		ImDrawList* draw_list = ImGui::GetWindowDrawList();
		static ImVec4 colf = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
		const ImU32 col = ImColor(colf);
		draw_list->AddLine(p1, p2, col, 2.0f);
	}
	ImGui::End();
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


void Controller::CloseGame() {
	view.getWindow().close();
}
void Controller::RunGame()
{
	sf::RenderWindow& window = view.getWindow();
	window.setFramerateLimit(30);
	window.setView(view.getView());

	ImGui::SFML::Init(window);

	ImGuiStyle& style = ImGui::GetStyle();
	style.FrameRounding = 5.0f;
	style.WindowRounding = 5.0f;
	style.Alpha = 0.825f;

	bool undoKeyPressed = false;
	bool redoKeyPressed = false;

	bool showOnlineOption = false;
	sf::Clock clock;
	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			ImGui::SFML::ProcessEvent(event);
			if (event.type == sf::Event::Closed)
				CloseGame();

			if (event.type == sf::Event::Resized)
				view.ReSizeView();
		}

		ImGui::SFML::Update(window, clock.restart());

		switch (gameState)
		{
		case GameState::MainMenu:
			ShowMainMenu(showOnlineOption);
			break;
		case GameState::WaitingForConnection:
			WaitForConnection();
			break;
		case GameState::PlayGame:
			ShowPlayGameScreen(undoKeyPressed, redoKeyPressed, window);
			break;
		case GameState::Pause:
			showPauseOrPlayButton();
			ShowTextInCentre(window, "The Game is Paused!");
			break;
		case GameState::End:
			ShowEndScreen(window);
			break;
		default:
			break;
		}
		window.clear();
		view.Display(model.getBoard());
		ImGui::SFML::Render(window);
		window.display();
	}
	ImGui::SFML::Shutdown();
}

void Controller::ShowMainMenu(bool& showOnlineOption)
{
	static bool use_work_area = true;
	static ImGuiWindowFlags flags = ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoSavedSettings | ImGuiConfigFlags_NavEnableKeyboard;

	const ImGuiViewport* viewport = ImGui::GetMainViewport();
	ImGui::SetNextWindowPos(use_work_area ? viewport->WorkPos : viewport->Pos);
	ImGui::SetNextWindowSize(use_work_area ? viewport->WorkSize : viewport->Size);

	ImGuiIO& io = ImGui::GetIO();
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
	if (ImGui::Begin("Fullscreen window", nullptr, flags))
	{
		float windowWidth = ImGui::GetWindowWidth();
		float windowCenterX = ImGui::GetWindowPos().x + windowWidth * 0.5f;

		int button_count = 4;
		// Center the buttons inside the window
		ImGui::SetCursorPos(ImVec2(windowCenterX - ImGui::CalcTextSize("PLAY ONLINE").x * 0.5f, ImGui::GetWindowHeight() / 2.0 - button_count * 8));

		if (ImGui::Button("PLAY ONLINE") || showOnlineOption)
		{
			showOnlineOption = true;
			int port = 2300;

			float inputWidth = 60.0f;

			ImGui::SetCursorPosX((windowCenterX - inputWidth) * 0.5f);
			ImGui::PushItemWidth(inputWidth);

			ImGui::SetCursorPos(ImVec2(windowCenterX - ImGui::CalcItemWidth() * 0.5f, ImGui::GetCursorPosY()));
			ImGui::InputInt("Create Port", &port, 0);
			ImGui::SetCursorPos(ImVec2(windowCenterX - ImGui::CalcTextSize("CREATE SERVER").x * 0.5f, ImGui::GetCursorPosY()));
			if (ImGui::Button("CREATE SERVER"))
			{
				unsigned short p = static_cast<unsigned short>(port);
				gameState = GameState::WaitingForConnection;
				CreateServer(p);
				ImGui::End();
				return;
			}
			//address
			float addresswidth = 100.0f;
			ImGui::SetCursorPosX((windowCenterX - addresswidth) * 0.5f);
			ImGui::PushItemWidth(addresswidth);
			ImGui::SetCursorPos(ImVec2(windowCenterX - ImGui::CalcItemWidth() * 0.5f, ImGui::GetCursorPosY()));
			static char address[128];
			ImGui::InputText("Address", address, sizeof(address));

			//port
			ImGui::SetCursorPosX((windowCenterX - inputWidth) * 0.5f);
			ImGui::PushItemWidth(inputWidth);

			ImGui::SetCursorPos(ImVec2(windowCenterX - ImGui::CalcItemWidth() * 0.5f, ImGui::GetCursorPosY()));
			ImGui::InputInt("Connect Port", &port, 0);

			ImGui::SetCursorPos(ImVec2(windowCenterX - ImGui::CalcTextSize("CONNECT TO SERVER").x * 0.5f, ImGui::GetCursorPosY()));
			if (ImGui::Button("CONNECT TO SERVER"))
			{
				unsigned short p = static_cast<unsigned short>(port);
				CreateClient(address, p);
				gameState = GameState::WaitingForConnection;
				ImGui::End();
				return;
			}
		}

		ImGui::SetCursorPos(ImVec2(windowCenterX - ImGui::CalcTextSize("PLAY LOCAL").x * 0.5f, ImGui::GetCursorPosY()));
		if (ImGui::Button("PLAY LOCAL"))
		{
			gameState = GameState::PlayGame;
		}

		ImGui::SetCursorPos(ImVec2(windowCenterX - ImGui::CalcTextSize("PLAY VS AI").x * 0.5f, ImGui::GetCursorPosY()));
		if (ImGui::Button("PLAY VS AI"))
		{
			// Handle button click
		}

		ImGui::SetCursorPos(ImVec2(windowCenterX - ImGui::CalcTextSize("EXIT").x * 0.5f, ImGui::GetCursorPosY()));
		if (ImGui::Button("EXIT"))
		{
			// Handle button click
			CloseGame();
		}
		std::string str = "Created by SyedSaifuddin045 (a.k.a Velcro,Lomdi).";
		ImVec2 TextLength = ImGui::CalcTextSize(str.c_str());
		ImGui::SetCursorPos(ImVec2(ImGui::GetWindowWidth() - TextLength.x, ImGui::GetWindowHeight() - TextLength.y - 4));
		ImGui::TextColored(ImVec4(0.9f, 0.8f, 0.0f, 1.0f), "%s", str.c_str());
	}
	ImGui::End();
}

void Controller::ShowPlayGameScreen(bool& undoKeyPressed, bool& redoKeyPressed, sf::RenderWindow& window)
{
	showPauseOrPlayButton();
	bool isKingCheck = currentPlayer->isInCheck();
	auto validMoves = currentPlayer->getValidMoves();
	if (validMoves.size() <= 0)
	{
		checkMate = true;
		gameState = GameState::End;
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

	ShowUndoAndRedoButtons(window);
	ShowTurnText(window);

	ImGui::ShowDemoWindow();
	ImGui::End();

	HandleInput(window, isKingCheck);
}

void Controller::ShowEndScreen(sf::RenderWindow& window)
{
	Global::Color winColor = (currentPlayer->getColor() == Global::Color::white) ? Global::Color::black : Global::Color::white;
	std::string str = "GAME OVER!\n" + Global::GetStringFromColor(winColor) + " wins.";
	ShowTextInCentre(window, str);
}

void Controller::ShowTurnText(sf::RenderWindow& window)
{
	ImGuiWindowFlags flags = ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoSavedSettings;

	std::string turnText = "TURN : " + Global::GetStringFromColor(currentPlayer->getColor());
	ImGui::SetNextWindowPos(ImVec2(window.getSize().x / 2 - turnText.size() / 2, window.getSize().y - 25), ImGuiCond_Always);

	ImVec2 windowPos(window.getSize().x / 2.0f, window.getSize().y - 22);
	ImVec2 windowSize(window.getSize().x, 20);
	ImGui::SetNextWindowPos(ImVec2(windowPos.x - windowSize.x / 2, windowPos.y - windowSize.y / 2), ImGuiCond_Always);
	ImGui::SetNextWindowSize(windowSize, ImGuiCond_Always);
	ImGui::Begin("Game Paused", nullptr, flags);
	ImVec2 textSize = ImGui::CalcTextSize(turnText.c_str());
	ImGui::SetCursorPosX((ImGui::GetWindowContentRegionMax().x + ImGui::GetWindowContentRegionMin().x - textSize.x) * 0.5f);
	ImGui::SetCursorPosY((ImGui::GetWindowContentRegionMax().y + ImGui::GetWindowContentRegionMin().y - textSize.y) * 0.5f);
	ImGui::Text("%s", turnText.c_str());
	ImGui::End();
}

void Controller::ShowUndoAndRedoButtons(sf::RenderWindow& window)
{
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
}

void Controller::showPauseOrPlayButton()
{
	ImGuiWindowFlags flags = ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoBackground;

	ImGui::SetNextWindowPos(ImVec2(0, 0), ImGuiCond_Always);

	ImGui::Begin("Pause/Resume", 0, flags);
	if (isPaused)
	{
		if (ImGui::Button("PLAY"))
		{
			isPaused = false;
			gameState = GameState::PlayGame;
		}
	}
	else
	{
		if (ImGui::Button("PAUSE"))
		{
			isPaused = true;
			gameState = GameState::Pause;
		}
	}
	ImGui::End();
}

void Controller::ShowTextInCentre(sf::RenderWindow& window, std::string s)
{
	ImVec2 windowPos(window.getSize().x / 2.0f, window.getSize().y / 2.0f);
	ImVec2 windowSize(window.getSize().x, 90);
	ImGui::SetNextWindowPos(ImVec2(windowPos.x - windowSize.x / 2, windowPos.y - windowSize.y / 2), ImGuiCond_Always);
	ImGui::SetNextWindowSize(windowSize, ImGuiCond_Always);
	ImGui::Begin("Game Paused", nullptr, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);

	// Center the text inside the window
	ImVec2 textSize = ImGui::CalcTextSize(s.c_str());
	ImGui::SetCursorPosX((ImGui::GetWindowContentRegionMax().x + ImGui::GetWindowContentRegionMin().x - textSize.x) * 0.5f);
	ImGui::SetCursorPosY((ImGui::GetWindowContentRegionMax().y + ImGui::GetWindowContentRegionMin().y - textSize.y) * 0.5f);
	ImGui::Text("%s", s.c_str());

	float buttonPosX = (ImGui::GetWindowContentRegionMax().x + ImGui::GetWindowContentRegionMin().x) * 0.5f - ImGui::CalcTextSize("Restart   ").x * 0.5f - ImGui::CalcTextSize("   Exit").x * 0.5f;
	ImGui::SetCursorPosX(buttonPosX);
	ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 10.0f);

	if (ImGui::SmallButton("Restart"))
	{
		model.Reset();
		SetUpController();
	}
	ImGui::SameLine();
	if (ImGui::SmallButton("Exit"))
	{
		CloseGame();
	}
	ImGui::End();

}

Controller::~Controller()
{
}