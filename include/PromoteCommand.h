#pragma once

#include <Command.h>
#include <Piece.h>

class GameModel;
class PromoteCommand :public Command {

public:
	PromoteCommand(GameModel& model,sf::Vector2i position,Global::Piece_Type pieceType);
	void execute() override;
	void undo() override;
private:
	GameModel& model;
	sf::Vector2i position;
	Global::Piece_Type pieceType;
	std::shared_ptr<Piece> previousPiece;
	std::shared_ptr<Piece> newPiece;
};