#ifndef KING_H
#define KING_H

#include <vector>
#include "../piece.h"

class Move;
class GameState;

class King : public Piece {

public:
	King(bool isWhite);

	std::vector<Move> possibleMoves(int x, int y, const GameState& gameState) const override;

};

#endif
