#pragma once
#include <SFML/Graphics.hpp>
#include <memory>

#include <Log.h>

namespace Global
{
	enum class Piece_Type
	{
		Null, Pyada, Haanthi, Ghoda, Unth, Wazir, Raaja
	};

	enum class Color {
		null,black, white
	};

	static Piece_Type PieceTypeFromString(const std::string& str)
	{

		if (str.empty() || str == "Null")
			return Piece_Type::Null;
		else if (str == "Pyada")
			return Piece_Type::Pyada;
		else if (str == "Haanthi")
			return Piece_Type::Haanthi;
		else if (str == "Ghoda")
			return Piece_Type::Ghoda;
		else if (str == "Unth")
			return Piece_Type::Unth;
		else if (str == "Wazir")
			return Piece_Type::Wazir;
		else if (str == "Raaja")
			return Piece_Type::Raaja;
	}

	static std::string GetStringFromPieceType(Piece_Type pType)
	{
		switch (pType)
		{
		case Global::Piece_Type::Pyada:
			return "Pyada";
			break;
		case Global::Piece_Type::Haanthi:
			return "Haanthi";
			break;
		case Global::Piece_Type::Ghoda:
			return "Ghoda";
			break;
		case Global::Piece_Type::Unth:
			return "Unth";
			break;
		case Global::Piece_Type::Wazir:
			return "Wazir";
			break;
		case Global::Piece_Type::Raaja:
			return "Raaja";
			break;
		case Global::Piece_Type::Null:
		default:
			return "Null";
			break;
		}
	}

	static std::string GetStringFromColor(Color color)
	{
		switch (color)
		{
		case Global::Color::black:
			return "black";
			break;
		case Global::Color::white:
			return "white";
			break;
		case Global::Color::null:
		default:
			return "";
			break;
		}
	}

	static Color GetColorFromString(const std::string str)
	{
		if (str.empty())
			return Color::null;
		if (str == "white")
			return Color::white;
		if (str == "black")
			return Color::black;
	}
}

class Piece {
public:
	Piece(sf::Vector2f size, std::shared_ptr<sf::Texture>& texture, Global::Piece_Type p_type,Global::Color color);
	Piece();
	void Render(sf::RenderWindow& window);

	//Getter and Setter
	sf::RectangleShape& getBody();
	Global::Piece_Type getPieceType();
	Global::Color getPieceColor();
	bool doesUseShader() { return p_UseShader; }
	void setPieceSelected(bool b);
	void setDangerHighlight(bool b);
	void setUseShader(bool b);
	void setPosition(sf::Vector2f Position);

	bool isPieceSelected();
	void DeselectPiece();
	void pieceToggleSelection();
private:
	sf::RectangleShape p_Body;
	std::shared_ptr<sf::Texture> p_Texture;
	std::shared_ptr<sf::Shader> shader;

	Global::Piece_Type p_Type;
	Global::Color p_Color;
	bool p_UseShader;
	bool p_Selected;
};