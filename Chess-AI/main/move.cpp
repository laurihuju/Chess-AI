#include "include.h"

Move::Move(int x1, int y1, int x2, int y2) : _x1(x1), _y1(y1), _x2(x2), _y2(y2) {}

Move::Move(const std::string& input) {
	if (input.length() != 4) {
		_x1 = 0;
		_y1 = 0;

		_x2 = 0;
		_y2 = 0;

		return;
	}

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

