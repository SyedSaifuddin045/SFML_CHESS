#include "Client.h"
#include "GameModel.h"

Client::Client(GameModel& model)
	:model(model)
{
}

void Client::Connect(const char* address, unsigned short port)
{
	if (socket.connect(address, port) == sf::Socket::Done) {
		std::cout << "Connected to the server at " << address << ":" << port << std::endl;
		isConnected = true;
	}
	else {
		std::cerr << "Failed to connect to the server at " << address << ":" << port << std::endl;
		isConnected = false;
		return;
	}
	std::thread receiveThread(&Client::RecieveMessage, this);
	receiveThread.detach();
}

void Client::sendMessage(const std::string& message)
{
	if (isConnected)
	{
		sf::Packet packet;
		packet << message;
		if (socket.send(packet) != sf::Socket::Done)
		{
			std::cerr << "Error : Failed to send packet to sserver!" << std::endl;
		}
	}
	else
	{
		std::cerr << "Client Not Connected to server!" << std::endl;
	}
}

void Client::RecieveMessage()
{
	while (isConnected)
	{
		sf::Packet packet;
		if (socket.receive(packet) != sf::Socket::Done)
		{
			std::string message;
			packet >> message;
			std::cout << "Recieved Packet : " << message << std::endl;
		}
		else
		{
			std::cerr << "Error : recieving Packet from the server!" << std::endl;
		}
	}
}

//Example : "MOVE from 7,1 to 5,2"
void Client::ProcessString(const std::string& s)
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