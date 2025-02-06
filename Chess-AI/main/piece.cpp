#include <typeinfo>
#include "piece.h"

bool Piece::operator==(const Piece& other) const {
	return typeid(other) == typeid(*this) && isWhite() == other.isWhite();
}

bool Piece::operator!=(const Piece& other) const {
	return !(*this == other);
}

Piece::Piece(bool isWhite) : _isWhite(isWhite) {}
Piece::~Piece() {}

bool Piece::isWhite() const {
	return _isWhite;
}
