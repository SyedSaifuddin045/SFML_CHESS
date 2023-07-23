#include "MoveCommand.h"
#include "GameModel.h"

MoveCommand::MoveCommand(GameModel& model, std::shared_ptr<Piece> piece, const sf::Vector2i& newPosition, const sf::Vector2i& oldPosition)
	:piece(piece),newPosition(newPosition),oldPosition(oldPosition),model(model)
{
}

void MoveCommand::execute()
{
	auto pair = std::make_pair(sf::Vector2i(-1, -1), std::make_shared<Piece>(Piece()) );
	capturedPieceInfo =  model.MovePiece(piece, newPosition,pair);
}

void MoveCommand::undo()
{
	this->model.MovePiece(piece, oldPosition,capturedPieceInfo);
}
