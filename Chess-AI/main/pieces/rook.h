#ifndef ROOK_H
#define ROOK_H

#include <vector>
#include "../piece.h"

class Move;
class GameState;

class Rook : public Piece {

public:
	Rook(bool isWhite);

	std::vector<Move> possibleMoves(int x, int y, const GameState& gameState) const override;

};

#endif
