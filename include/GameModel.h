#pragma once
#include <vector>
#include <memory>
#include <Tile.h>

class GameModel {
public:
    GameModel();
    std::pair<Piece_Type,sf::String> GetPieceForPosiition(int row, int col);
    void InitializeBoard();
    std::vector<std::vector<Tile>> getBoard() { return Board; }
public:
    const int rows = 8;
    const int cols = 8;
    const float tile_size = 80.0f;
private:
    std::vector<std::vector<Tile>> Board;
    std::shared_ptr<sf::Texture> WhiteTexture;
    std::shared_ptr<sf::Texture> BlackTexture;
};