#ifndef BISHOP_H
#define BISHOP_H

#include <vector>
#include "../piece.h"

class Move;
class GameState;

class Bishop : virtual public Piece {

public:
	Bishop(bool isWhite);

	std::vector<Move> possibleMoves(int x, int y, const GameState& gameState) const override;

};

#endif
