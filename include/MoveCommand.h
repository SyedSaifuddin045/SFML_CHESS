#pragma once
#include <Command.h>
#include <Piece.h>
#include <SFML/Graphics.hpp>

class GameModel;

class MoveCommand :public Command
{
public:
	MoveCommand(GameModel& model, std::shared_ptr<Piece> piece, const sf::Vector2i& newPosition, const sf::Vector2i& oldPosition);
	void execute() override;
	void undo() override;

private:
	GameModel& model;
	std::shared_ptr<Piece> piece;
	sf::Vector2i newPosition;
	sf::Vector2i oldPosition;
	std::pair<sf::Vector2i, std::shared_ptr<Piece>> capturedPieceInfo;
};