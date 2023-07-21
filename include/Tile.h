#pragma once
#include <SFML/Graphics.hpp>

#include <Piece.h>
#include <PieceFactory.h>

class Tile {
public:
    Tile();
    Tile(sf::Vector2f size, std::shared_ptr<sf::Texture> normalTexture, std::shared_ptr<sf::Texture> highlightTexture, sf::Vector2f Position, sf::Vector2i boardPosition);
    void Render(sf::RenderWindow& window);
    void setPiece(std::shared_ptr<Piece> p);
    void unsetPiece();
    std::shared_ptr<Piece> getPiece();
    void setScreenPosition(sf::Vector2f& position);
    sf::Vector2f getScreenPosition() const;
    void setTexture(std::shared_ptr<sf::Texture> newtexture);

    void setNormalTexture();
    sf::Vector2i getGamePosition();
    void setHighlightTexture();

    bool isHighlighted;
private:
    sf::RectangleShape t_Body;
    sf::Vector2i t_Position;
    std::shared_ptr<Piece> piece;
    std::shared_ptr<sf::Texture> normalTexture;
    std::shared_ptr<sf::Texture> highlightTexture;
};