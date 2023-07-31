#include "PromoteCommand.h"
#include "GameModel.h"
#include <PieceFactory.h>

PromoteCommand::PromoteCommand(GameModel& model, sf::Vector2i position, Global::Piece_Type pieceType)
	:model(model),position(position),pieceType(pieceType)
{
	previousPiece =  this->model.getPieceAtPosition(position);
	newPiece = std::make_shared<Piece>(PieceFactory::CreatePiece(pieceType, previousPiece->getPieceColor()));
}

void PromoteCommand::execute()
{
	auto find = model.getPositioOccupiedOnBoard().find(position);
	if (find != model.getPositioOccupiedOnBoard().end())
	{
		model.getBoard()[position.x][position.y].unsetPiece();
		model.getBoard()[position.x][position.y].setPiece(newPiece);
		find->second = newPiece;
	}
}

void PromoteCommand::undo()
{
	auto find = model.getPositioOccupiedOnBoard().find(position);
	if (find != model.getPositioOccupiedOnBoard().end())
	{
		model.getBoard()[position.x][position.y].unsetPiece();
		model.getBoard()[position.x][position.y].setPiece(previousPiece);
		find->second = previousPiece;
	}
}