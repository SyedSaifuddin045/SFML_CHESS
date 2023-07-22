#include <memory>
#include <GameModel.h>

class Command {
public:
    virtual ~Command() {}
    virtual void execute() = 0;
    virtual void undo() = 0;
};

class MoveCommand :public Command
{
public:
    MoveCommand(GameModel& model, std::shared_ptr<Piece> piece, const sf::Vector2i& newPosition)
        : model(model), piece(piece), newPosition(newPosition), oldPosition(model.getPiecePosition(piece)) {}

    void execute() override {
        model.MovePiece(piece, newPosition);
    }

    void undo() override {
        model.MovePiece(piece, oldPosition);
    }

private:
    GameModel& model;
    std::shared_ptr<Piece> piece;
    sf::Vector2i newPosition;
    sf::Vector2i oldPosition;
};