#include "Tile.h"

Tile::Tile(sf::Vector2f Size, sf::Texture& texture,sf::Vector2f Position)
	:m_body(Size)
{
	m_body.setTexture(&texture);
	m_body.setPosition(Position);
	//m_body.setOrigin(Size / 2.0f);
}

void Tile::Render(sf::RenderWindow& window)
{
	window.draw(m_body);
}
