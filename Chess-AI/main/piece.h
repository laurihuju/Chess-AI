#ifndef PIECE_H
#define PIECE_H

#include <vector>

class Move;
class GameState;

class Piece {

private:
	bool _isWhite;

public:
	Piece(bool isWhite);
	virtual ~Piece();

	bool isWhite() const;

	virtual std::vector<Move> possibleMoves(int x, int y, const GameState& gameState) const = 0;

};

#endif
