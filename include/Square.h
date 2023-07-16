#pragma once
#include <SFML/Graphics.hpp>

#include <Piece.h>

class Square {
public:
	Square(sf::Vector2f size,sf::Texture* texture);
private:
	sf::RectangleShape m_body;
	Piece piece;
};