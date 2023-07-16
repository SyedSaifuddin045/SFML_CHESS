#include "Piece.h"

Piece::Piece(sf::Vector2f size, sf::Texture* texture,Piece_Type p_type)
	:m_body(size)
{
	m_body.setTexture(texture);
	m_pType = p_type;
}
Piece::Piece()
{
	m_body = sf::RectangleShape();
	m_pType = Piece_Type::Null;
}