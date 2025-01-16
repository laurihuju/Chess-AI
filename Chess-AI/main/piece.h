#pragma once

class Piece {

private:
	bool _isAI;

public:
	Piece(bool isAI) : _isAI(isAI) {}

	bool isAI() {
		return _isAI;
	}

};
