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

    void possibleMoves(std::vector<Move>& moves, int x, int y, const GameState& gameState) const override;

	/// <summary>
    /// Checks if this piece at the given location threatens the given square.
    /// </summary>
    /// <param name="ownX"></param>
    /// <param name="ownY"></param>
    /// <param name="squareX"></param>
    /// <param name="squareY"></param>
    /// <param name="gameState"></param>
    /// <returns></returns>
    bool threatensSquare(int ownX, int ownY, int squareX, int squareY, const GameState& gameState) const override;

};

#endif
