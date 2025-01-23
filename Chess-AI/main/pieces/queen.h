#ifndef QUEEN_H
#define QUEEN_H

#include <vector>
#include "../piece.h"

class Move;
class GameState;

class Queen : public Piece {

public:
	Queen(bool isWhite);

	std::vector<Move> possibleMoves(int x, int y, const GameState& gameState) const override;

};

#endif
