#include "MoveCommand.h"
#include "GameModel.h"

MoveCommand::MoveCommand(GameModel& model, std::shared_ptr<Piece> piece, const sf::Vector2i& newPosition, const sf::Vector2i& oldPosition)
	:piece(piece),newPosition(newPosition),oldPosition(oldPosition),model(model)
{
}

void MoveCommand::execute()
{
	this->model.MovePiece(piece, newPosition);
}

void MoveCommand::undo()
{
	this->model.MovePiece(piece, oldPosition);
}
