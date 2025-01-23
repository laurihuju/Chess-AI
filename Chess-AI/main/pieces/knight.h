#ifndef KNIGHT_H
#define KNIGHT_H

#include <vector>
#include "../piece.h"

class Move;
class GameState;

class Knight : public Piece {

public:
	Knight(bool isWhite);

	std::vector<Move> possibleMoves(int x, int y, const GameState& gameState) const override;

};

#endif
