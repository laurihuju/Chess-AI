#ifndef QUEEN_H
#define QUEEN_H

#include <vector>
#include "../piece.h"
#include "bishop.h"
#include "rook.h"

class Move;
class GameState;

/// <summary>
/// A class that describes a queen piece.
/// Queen is inherited from bishop and rook as it has the moves of the both piece types.
/// The class is immutable, and any child class should be immutable.
/// </summary>
class Queen : public Bishop, public Rook {

public:
    /// <summary>
    /// Initializes this queen.
    /// </summary>
    /// <param name="isWhite">If the queen is white</param>
    Queen(bool isWhite);

    /// <summary>
    /// The PieceType of this piece.
    /// </summary>
    /// <returns></returns>
    PieceType getType() const override;

    /// <summary>
    /// Adds all the possible moves of this piece to the moves vector. Does not take into account if the king is threatened.
    /// </summary>
    /// <param name="moves"></param>
    /// <param name="x">The x coordinate of this piece</param>
    /// <param name="y">The y coordinate of this piece</param>
    /// <param name="gameState">The game state from which the possible moves are generated</param>
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

    /// <summary>
    /// The evaluation value of this piece at the given coordinates on the given GameState.
    /// </summary>
    /// <param name="gameState">The game state of evaluation</param>
    /// <param name="x">The X coordinate of the piece</param>
    /// <param name="y">The Y coordinate of the piece</param>
    /// <returns></returns>
    int evaluationValue(const GameState& gameState, int x, int y) const override;

};

#endif
