#pragma once
#include <SFML/Graphics.hpp>

#include <Piece.h>

class Tile {
public:
	Tile(sf::Vector2f size,sf::Texture& texture, sf::Vector2f Position);
	void Render(sf::RenderWindow& window);
private:
	sf::RectangleShape m_body;
	Piece piece;
};