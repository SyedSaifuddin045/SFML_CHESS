#include "Tile.h"

Tile::Tile()
{
}

Tile::Tile(sf::Vector2f size, std::shared_ptr<sf::Texture> normalTexture, std::shared_ptr<sf::Texture> highlightTexture, sf::Vector2f Position,sf::Vector2i boardPosition)
	: t_Body(size), normalTexture(normalTexture), highlightTexture(highlightTexture)
{
	t_Body.setPosition(Position);
	t_Position = boardPosition;
	setTexture(normalTexture); // Set the normal texture initially
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

void Tile::setTexture(std::shared_ptr<sf::Texture> newtexture)
{
	t_Body.setTexture(newtexture.get());
}

void Tile::setNormalTexture()
{
	setTexture(normalTexture);
}

void Tile::setHighlightTexture()
{
	setTexture(highlightTexture);
}
