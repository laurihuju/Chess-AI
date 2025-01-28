#ifndef ROOK_H
#define ROOK_H

#include <vector>
#include "../piece.h"

class Move;
class GameState;

class Rook : virtual public Piece {

public:
	Rook(bool isWhite);

	void possibleMoves(std::vector<Move>& moves, int x, int y, const GameState& gameState) const override;

};

#endif
