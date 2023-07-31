#pragma once
#include <SFML/Network.hpp>
#include <iostream>
#include <thread>

class GameModel;

class Server
{
public:
	Server(GameModel& model, unsigned short port);
	void ProcessString(std::string& s);
	void start();
	void Run();
	bool Server::IsClientConnected() const { return isConnected; }
	void sendMessage(const std::string& message);
private:
	void handleClientMessage();
	void runServerThread();
private:
	sf::TcpListener listener;
	sf::TcpSocket clientSocket;

	unsigned short port;
	bool isRunning;
	bool isConnected;

	GameModel& model;
};