#ifndef piece_h
#define piece_h

#include "include.h"

class Piece {

private:
	bool _isWhite;

public:
	Piece(bool isWhite);
	virtual ~Piece();

	bool isWhite();

	virtual std::vector<Move> possibleMoves(int x, int y, const GameState& gameState) const {

	}

};

#endif
