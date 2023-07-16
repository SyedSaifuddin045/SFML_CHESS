#include "Square.h"

Square::Square(sf::Vector2f size, sf::Texture* texture)
	:m_body(size)
{
	m_body.setTexture(texture);
}
