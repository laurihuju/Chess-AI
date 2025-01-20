#include "piece.h"

Piece::Piece(bool isWhite) : _isWhite(isWhite) {}

bool Piece::isWhite() {
	return _isWhite;
}
