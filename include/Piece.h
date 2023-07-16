#pragma once
#include <SFML/Graphics.hpp>
#include <memory>
#include <Log.h>

enum class Piece_Type
{
	Null, Pyada, Haanthi, Ghoda, Unth, Wazir, Raaja
};

class Piece {
public:
	Piece(sf::Vector2f size, std::shared_ptr<sf::Texture> texture, Piece_Type p_type);
	Piece();
	void Render(sf::RenderWindow& window);
	Piece_Type getPieceType();
	void setPosition(sf::Vector2f Position);
	sf::RectangleShape& getBody();
	bool getApplyShader() { return p_applyShader; }
	void setApplyShader(bool b) { p_applyShader = b; }
private:
	sf::RectangleShape p_Body;
	std::shared_ptr<sf::Texture> p_Texture;
	Piece_Type p_Type;
	bool p_applyShader;
};

class PieceFactory {
public:
	static Piece CreatePiece(Piece_Type p_type, const sf::String& color)
	{
		sf::Vector2f size = sf::Vector2f(80.0f, 80.0f);
		std::shared_ptr<sf::Texture> texture = std::make_shared<sf::Texture>();

		switch (p_type)
		{
		case Piece_Type::Pyada:
			if (!texture->loadFromFile(RESOURCE "/textures/" + color + "/Pyada.png"))
				std::cout << "Failed to load " << color.toAnsiString() << " Pyada texture." << std::endl;
			break;

		case Piece_Type::Haanthi:
			if (!texture->loadFromFile(RESOURCE "/textures/" + color + "/Haanthi.png"))
				std::cout << "Failed to load " << color.toAnsiString() << " Haanthi texture." << std::endl;
			break;

		case Piece_Type::Ghoda:
			if (!texture->loadFromFile(RESOURCE "/textures/" + color + "/Ghoda.png"))
				std::cout << "Failed to load " << color.toAnsiString() << " Ghoda texture." << std::endl;
			break;

		case Piece_Type::Unth:
			if (!texture->loadFromFile(RESOURCE "/textures/" + color + "/Unth.png"))
				std::cout << "Failed to load " << color.toAnsiString() << " Unth texture." << std::endl;
			break;

		case Piece_Type::Wazir:
			if (!texture->loadFromFile(RESOURCE "/textures/" + color + "/Wazir.png"))
				std::cout << "Failed to load " << color.toAnsiString() << " Wazir texture." << std::endl;
			break;

		case Piece_Type::Raaja:
			if (!texture->loadFromFile(RESOURCE "/textures/" + color + "/Raaja.png"))
				std::cout << "Failed to load " << color.toAnsiString() << " Raaja texture." << std::endl;
			break;

		default:
		case Piece_Type::Null:
			return Piece();
			break;
		}
		texture.get()->setSmooth(true);
		return Piece(size, texture, p_type);
	}
};
