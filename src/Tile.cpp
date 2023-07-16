#include "Tile.h"

Tile::Tile()
{
}

Tile::Tile(sf::Vector2f Size, std::shared_ptr<sf::Texture> texture,sf::Vector2f Position)
	:p_Body(Size)
{
	p_Body.setTexture(texture.get());
	p_Body.setPosition(Position);
	//m_body.setOrigin(Size / 2.0f);
}

void Tile::Render(sf::RenderWindow& window)
{
	window.draw(p_Body);
	if (piece.getPieceType() != Piece_Type::Null)
	{
		piece.Render(window);
	}
}

void Tile::setPiece(const Piece& p)
{
	this->piece = p;
	this->piece.setPosition(p_Body.getPosition());
}

Piece Tile::getPiece()
{
	return this->piece;
}
