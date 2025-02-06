#ifndef PAWN_H
#define PAWN_H

#include <vector>
#include "../piece.h"

class Move;
class GameState;

/// <summary>
/// A class that describes a pawn piece.
/// </summary>
class Pawn : public Piece {

public:
	/// <summary>
	/// Initializes this pawn.
	/// </summary>
	/// <param name="isWhite">If the pawn is white</param>
	Pawn(bool isWhite);

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
	/// Creates a copy of this piece.
	/// </summary>
	/// <returns>Pointer to the copied piece</returns>
	Piece* clone() const override;

};

#endif
