#pragma once
#include <SFML/Graphics.hpp>

enum class Piece_Type
{
	Null,Pyada,Haanthi,Ghoda,Unth,Wazir,Raaja
};

class Piece {
public:
	Piece(sf::Vector2f size,sf::Texture* texture,Piece_Type p_type);
	Piece();
private:
	sf::RectangleShape m_body;
	Piece_Type m_pType;
};