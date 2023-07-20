#include "Tile.h"

Tile::Tile()
{
}

Tile::Tile(sf::Vector2f size, std::shared_ptr<sf::Texture> texture, sf::Vector2f position, sf::Vector2i boardPosition)
	:t_Body(size)
{
	t_Body.setTexture(texture.get());
	t_Body.setPosition(position);
	t_Position = boardPosition;
	//m_body.setOrigin(Size / 2.0f);
}

void Tile::Render(sf::RenderWindow& window)
{

	window.draw(t_Body);
	if (piece.getPieceType() != Global::Piece_Type::Null)
	{
		piece.Render(window);
	}

}

void Tile::setPiece(Piece& p)
{
	this->piece = p;
	this->piece.setPosition(t_Body.getPosition());
}

Piece& Tile::getPiece()
{
	return this->piece;
}

void Tile::setScreenPosition(sf::Vector2f& position)
{
	t_Body.setPosition(position);
}

sf::Vector2f Tile::getScreenPosition() const
{
	return t_Body.getPosition();
}

void Tile::setHighlight(std::shared_ptr<sf::Texture> highlight_texture)
{
	t_Body.setTexture(highlight_texture.get());
}