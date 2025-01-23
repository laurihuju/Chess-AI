#ifndef QUEEN_H
#define QUEEN_H

#include <vector>
#include "../piece.h"
#include "bishop.h"
#include "rook.h"

class Move;
class GameState;

class Queen : public Bishop, public Rook {
public:
    Queen(bool isWhite);
    std::vector<Move> possibleMoves(int x, int y, const GameState& gameState) const override;
};

#endif
