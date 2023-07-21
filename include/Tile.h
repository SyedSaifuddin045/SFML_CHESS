#pragma once
#include <SFML/Graphics.hpp>

#include <Piece.h>

class Tile {
public:
    Tile();
    Tile(sf::Vector2f size, std::shared_ptr<sf::Texture> normalTexture, std::shared_ptr<sf::Texture> highlightTexture, sf::Vector2f Position, sf::Vector2i boardPosition);
    void Render(sf::RenderWindow& window);
    void setPiece(Piece& p);
    Piece& getPiece();
    void setScreenPosition(sf::Vector2f& position);
    sf::Vector2f getScreenPosition() const;
    void setTexture(std::shared_ptr<sf::Texture> newtexture);

    void setNormalTexture();

    void setHighlightTexture();

private:
    sf::RectangleShape t_Body;
    sf::Vector2i t_Position;
    Piece piece;
    std::shared_ptr<sf::Texture> normalTexture;
    std::shared_ptr<sf::Texture> highlightTexture;
};