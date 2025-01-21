#pragma once
#include "include.h"

class Piece {

private:
	bool _isWhite;

public:
	Piece(bool isWhite);
	virtual ~Piece();

	bool isWhite();

};
