#pragma once

#include <SFML/Graphics.hpp>
#include <memory>
#include <string>
#include <unordered_map>

#include "Log.h"
#include "Piece.h"

class PieceFactory {
public:
	static Piece& CreatePiece(Global::Piece_Type p_type, Global::Color color);
};
