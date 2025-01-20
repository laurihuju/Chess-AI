#include "piece.h"

Piece::Piece(bool isWhite) : _isWhite(isWhite) {}
Piece::~Piece() {}

bool Piece::isWhite() {
	return _isWhite;
}
