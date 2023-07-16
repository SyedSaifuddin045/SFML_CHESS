#include "Piece.h"
#include <Log.h>

Piece::Piece(sf::Vector2f size, std::shared_ptr<sf::Texture> texture, Piece_Type p_type)
	: p_Body(size), p_Texture(texture), p_Type(p_type)
{
	p_Body.setTexture(p_Texture.get());
	setApplyShader(false);
}

Piece::Piece()
{
	p_Type = Piece_Type::Null;
}

void Piece::Render(sf::RenderWindow& window)
{
	if (p_applyShader)
	{
		std::cout << "Rendering with shader" << std::endl;
		window.draw(this->p_Body);
	}
	else
	{
		window.draw(this->p_Body);
	}
}

Piece_Type Piece::getPieceType()
{
	return p_Type;
}

void Piece::setPosition(sf::Vector2f Position)
{
	this->p_Body.setPosition(Position);
}

sf::RectangleShape& Piece::getBody()
{
	return this->p_Body;
}
