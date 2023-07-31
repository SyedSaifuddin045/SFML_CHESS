#include "Piece.h"
#include <Log.h>

Piece::Piece(sf::Vector2f size, std::shared_ptr<sf::Texture>& texture, Global::Piece_Type p_type,Global::Color color)
	: p_Body(size), p_Texture(texture), p_Type(p_type)
{
	this->p_Body.setTexture(p_Texture.get());
	shader = std::make_shared<sf::Shader>();
	p_UseShader = false;
	p_Selected = false;
	p_Color = color;
}

Piece::Piece()
{
	p_Type = Global::Piece_Type::Null;
	shader = std::make_shared<sf::Shader>();
}

void Piece::setPieceSelected(bool b)
{
	this->p_UseShader = b;
	this->p_Selected = b;
	if (b == true)
	{
		if (!shader->loadFromFile("res/shaders/fresnel.frag", sf::Shader::Fragment))
		{
			// Failed to load shader
			std::cout << "Failed to load piece highlight shader" << std::endl;
		}
		shader->setUniform("edgeColor", sf::Glsl::Vec4(1.0f, 1.0f, 0.0f, 0.8f));    // Set the highlight color
		shader->setUniform("textureSize", sf::Glsl::Vec2(256.0f, 256.0f));
		shader->setUniform("boundaryWidth", 2.00f);
		shader->setUniform("textureSampler", sf::Shader::CurrentTexture);
	}
}

void Piece::setDangerHighlight(bool b)
{
	p_UseShader = b;

	if (b == true)
	{
		if (!shader->loadFromFile("res/shaders/RedHighlight.frag", sf::Shader::Fragment))
		{
			// Failed to load shader
			std::cout << "Failed to load piece  Red highlight shader" << std::endl;
		}
		shader->setUniform("redIncrease", 0.5f);
		shader->setUniform("texture", sf::Shader::CurrentTexture);
	}
}

void Piece::setUseShader(bool b)
{
	p_UseShader = false;
}

void Piece::pieceToggleSelection()
{
	if (this->p_Selected == true)
		setPieceSelected(false);
	else
		setPieceSelected(true);
}

Global::Color Piece::getPieceColor()
{
	return this->p_Color;
}

void Piece::Render(sf::RenderWindow& window)
{
	if (p_UseShader == true)
	{
		window.draw(this->p_Body, shader.get());
	}
	else
	{
		window.draw(this->p_Body);
	}
}

Global::Piece_Type Piece::getPieceType()
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

bool Piece::isPieceSelected()
{
	return p_Selected;
}

void Piece::DeselectPiece()
{
	setPieceSelected(false);
}
