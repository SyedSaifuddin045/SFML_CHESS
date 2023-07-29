#pragma once
#include <iostream>
#include <thread>

#include <SFML/Network.hpp>

class GameModel;

class Client {
public:
	Client(GameModel& model);
	void Connect(const char* address, unsigned short port);
	void sendMessage(const std::string& message);
	void RecieveMessage();
	void ProcessString(const std::string& s);
	bool isConnectedToServer() { return isConnected; }
private:
	sf::TcpSocket socket;

	bool isConnected = false;
	GameModel& model;
};