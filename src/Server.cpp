#include "Server.h"
#include <GameModel.h>

//Example : "MOVE from 7,1 to 5,2"
void Server::ProcessString(std::string& s)
{
	int i = -1;
	i = s.find("MOVE");
	if (i != -1)
	{
		int index = s.find("MOVE") + 10;
		int fromX = s[index] - '0';
		index += 2;
		int fromY = s[index] - '0';
		index += 5;
		int toX = s[index] - '0';
		index += 2;
		int toY = s[index] - '0';
		std::unique_ptr<Command> moveCommand = std::make_unique<MoveCommand>(model, model.getPieceAtPosition(sf::Vector2i(fromX, fromY)), sf::Vector2i(toX, toY), sf::Vector2i(fromX, fromY));
		model.executeCommand(std::move(moveCommand));
		sendMessage("ACK");
		return;
	}
	i = s.find("UNDO");
	if (i != -1)
	{
		model.undoLastCommand();
		sendMessage("ACK");
		return;
	}
	i = s.find("REDO");
	if (i != -1)
	{
		model.redoCommand();
		sendMessage("ACK");
		return;
	}

	std::cout << "No Valid Message from client!" << std::endl;
}

Server::Server(GameModel& model, unsigned short port)
	:port(port), model(model),isRunning(false),isConnected(false)
{
	if (listener.listen(port) != sf::Socket::Done)
	{
		std::cerr << "Error : failed to listen on port : " << port << std::endl;
	}
	else
	{
		std::cout << "Srever created on port : " << port << std::endl;
	}
}

void Server::Run()
{
	std::cout << "Waiting for Client To connect" << std::endl;
	while (isRunning)
	{
		if (listener.accept(clientSocket) == sf::Socket::Done)
		{
			std::cout << "Client Connected" << std::endl;
			isConnected = true;

			handleClientMessage();

			clientSocket.disconnect();
			isConnected = false;
			std::cout << "Client Disconnected!" << std::endl;
		}
		else
		{
			std::cout << "Server Listener Error!" << std::endl;
			isRunning = false;
		}
	}
}

void Server::handleClientMessage()
{
	while (true)
	{
		sf::Packet packet;

		if (clientSocket.receive(packet) != sf::Socket::Done)
		{
			std::cout << "Error Recieving packet!" << std::endl;
			break;
		}

		std::string message;
		packet >> message;
		ProcessString(message);
	}
}

void Server::sendMessage(const std::string& message)
{
	sf::Packet packet;
	packet << message;
	if (clientSocket.send(packet) != sf::Socket::Done)
	{
		std::cerr << "Error: Failed to send message to the client." << std::endl;
	}
}

void Server::start()
{
	// Start the server thread
	isRunning = true;
	std::thread serverThread(&Server::runServerThread, this);
	serverThread.detach();
}

void Server::runServerThread()
{
	Run();
}