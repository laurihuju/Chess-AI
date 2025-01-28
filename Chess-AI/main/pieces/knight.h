#ifndef KNIGHT_H
#define KNIGHT_H

#include <vector>
#include "../piece.h"

class Move;
class GameState;

class Knight : public Piece {

public:
	Knight(bool isWhite);

	void possibleMoves(std::vector<Move>& moves, int x, int y, const GameState& gameState) const override;

};

#endif
