#pragma once
#include <SFML/Graphics.hpp>

#include <Piece.h>

class Tile {
public:
	Tile();
	Tile(sf::Vector2f size,std::shared_ptr<sf::Texture> texture, sf::Vector2f Position);
	void Render(sf::RenderWindow& window);
	void setPiece(const Piece& p);
	Piece getPiece();
private:
	sf::RectangleShape p_Body;
	Piece piece;
};