#pragma once

class Piece {

private:
	bool _isWhite;

public:
	Piece(bool isWhite);
	virtual ~Piece();

	bool isWhite();

};
