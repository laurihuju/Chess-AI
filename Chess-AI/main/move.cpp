#include <string>
#include "move.h"

bool Move::operator==(const Move& other) const {
	return x1() == other.x1() && x2() == other.x2() && y1() == other.y1() && y2() == other.y2() && promotionPiece() == other.promotionPiece();
}

bool Move::operator!=(const Move& other) const {
	return !(*this == other);
}

Move::Move(int x1, int y1, int x2, int y2) : _x1(x1), _y1(y1), _x2(x2), _y2(y2), _promotionPiece(-1) {}

Move::Move(int x1, int y1, int x2, int y2, char promotionPiece) : _x1(x1), _y1(y1), _x2(x2), _y2(y2), _promotionPiece(promotionPiece) {}

Move::Move(const std::string& input) {
	// Parse promotion piece
	if (input.length() == 5) {
		if (input[4] != 'n' && input[4] != 'b' && input[4] != 'r' && input[4] != 'q') {
			_promotionPiece = 'q';
		}
		else {
			_promotionPiece = input[4];
		}
	}
	else {
		_promotionPiece = -1;
	}

	// Handle invalid length input
	if (input.length() != 4 && input.length() != 5) {
		_x1 = 0;
		_y1 = 0;

		_x2 = 0;
		_y2 = 0;

		return;
	}

	// Handle valid length input
	_x1 = convertXCoordinateFromInput(input[0]);
	_y1 = convertYCoordinateFromInput(input[1]);
	_x2 = convertXCoordinateFromInput(input[2]);
	_y2 = convertYCoordinateFromInput(input[3]);

}

int Move::x1() const {
	return _x1;
}
int Move::y1() const {
	return _y1;
}

int Move::x2() const {
	return _x2;
}
int Move::y2() const {
	return _y2;
}

char Move::promotionPiece() const {
	return _promotionPiece;
}

int Move::convertXCoordinateFromInput(char coordinateInputChar) const {
	if (coordinateInputChar < 'a' || coordinateInputChar > 'h')
		return 0;

	return (int)coordinateInputChar - 'a';
}
int Move::convertYCoordinateFromInput(char coordinateInputChar) const {
	if (coordinateInputChar < '1' || coordinateInputChar > '8')
		return 0;

	return '8' - (int)coordinateInputChar;
}

