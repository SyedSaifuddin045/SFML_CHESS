#pragma once
#include <vector>
#include <memory>
#include <Tile.h>
#include <PieceFactory.h>

namespace std {
    template<>
    struct hash<sf::Vector2i>
    {
        size_t operator()(const sf::Vector2i& key)const
        {
            return hash<int>()(key.x) ^ hash<int>()(key.y);
        }
    };
}
class GameModel {
public:
    GameModel();
    std::pair<Global::Piece_Type,Global::Color> GetPieceForPosiition(int row, int col);
    void InitializeBoard();
    std::vector<std::vector<Tile>>& getBoard() { return this->Board; }
    bool isOccupied(sf::Vector2i position);
    bool isPositionOccupiedByEnemy(sf::Vector2i position, Global::Color ourColor);
    void MovePiece(std::shared_ptr<Piece> piece, sf::Vector2i position);
    //sf::Vector2i getPiecePosition(std::shared_ptr<Piece>);
    const int rows = 8;
    const int cols = 8;
    const float tile_size = 80.0f;

    std::shared_ptr<sf::Texture> WhiteTexture;
    std::shared_ptr<sf::Texture> BlackTexture;
    std::shared_ptr<sf::Texture> WhiteHighlightTexture;
    std::shared_ptr<sf::Texture> BlackHighlightTexture;
private:
    std::vector<std::vector<Tile>> Board;
    std::unordered_map<sf::Vector2i, std::shared_ptr<Piece>> positionsOccupiedOnBoard;
    /*std::stack<Command*> executedCommands;
    std::stack<Command*> undoneCommands;*/
};