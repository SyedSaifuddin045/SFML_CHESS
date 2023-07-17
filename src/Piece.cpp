#include "Piece.h"
#include <Log.h>

Piece::Piece(sf::Vector2f size, std::shared_ptr<sf::Texture> texture, Piece_Type p_type)
	: p_Body(size), p_Texture(texture), p_Type(p_type)
{
	this->p_Body.setTexture(p_Texture.get());
	p_Selected = false;
	shader = std::make_shared<sf::Shader>();
}

Piece::Piece()
{
	p_Type = Piece_Type::Null;
	p_Selected = false;
}

void Piece::setPieceSelected(bool b)
{
	this->p_Selected = b;

	if (b == true)
	{
		if (!shader->loadFromFile(RESOURCE "/shaders/fresnel.frag", sf::Shader::Fragment))
		{
			// Failed to load shader
			std::cout << "Failed to load shader" << std::endl;
		}
		shader->setUniform("edgeColor", sf::Glsl::Vec4(1.0f, 1.0f, 0.0f, 0.8f));    // Set the highlight color
		shader->setUniform("textureSize", sf::Glsl::Vec2(256.0f, 256.0f));
		shader->setUniform("boundaryWidth", 2.00f);
		shader->setUniform("textureSampler", sf::Shader::CurrentTexture);
	}
}

void Piece::pieceToggleSelection()
{
	if (this->p_Selected == true)
		setPieceSelected(false);
	else
		setPieceSelected(true);
}

void Piece::Render(sf::RenderWindow& window)
{
	if (isPieceSelected() == true)
	{
		window.draw(this->p_Body, shader.get());
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

void Piece::DeselectPiece()
{
	setPieceSelected(false);
}
