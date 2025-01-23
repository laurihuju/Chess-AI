#ifndef PAWN_H
#define PAWN_H

#include <vector>
#include "../piece.h"

class Move;
class GameState;

class Pawn : public Piece {

public:
	Pawn(bool isWhite);

	std::vector<Move> possibleMoves(int x, int y, const GameState& gameState) const override;

};

#endif
