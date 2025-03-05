#ifndef KING_H
#define KING_H

#include <vector>
#include "../piece.h"

class Move;
class GameState;

/// <summary>
/// A class that describes a king piece.
/// The class is immutable, and any child class should be immutable.
/// </summary>
class King : public Piece {

public:
	/// <summary>
	/// Initializes this king.
	/// </summary>
	/// <param name="isWhite">If the king is white</param>
	King(bool isWhite);

	/// <summary>
	/// The PieceType of this piece.
	/// </summary>
	/// <returns></returns>
	PieceType getType() const override;

	/// <summary>
	/// The influence of this piece to the game phase.
	/// </summary>
	/// <returns></returns>
	char gamePhaseInfluence() const override;

	/// <summary>
	/// Adds the possible moves of this piece to the moves vector.
	/// Does not take into account if the king is threatened.
	/// You can generate only capture moves by setting the captureOnly parameter to true.
	/// </summary>
	/// <param name="moves">The vector where the moves will be added</param>
	/// <param name="x">The x coordinate of this piece</param>
	/// <param name="y">The y coordinate of this piece</param>
	/// <param name="gameState">The game state from which the possible moves are generated</param>
	/// <param name="captureOnly">If to generate only capture moves</param>
	void possibleMoves(std::vector<Move>& moves, char x, char y, const GameState& gameState, bool captureOnly = false) const override;

	/// <summary>
	/// The evaluation value of this piece at the given coordinates at the given game phase.
	/// </summary>
	/// <param name="x">The X coordinate of the piece</param>
	/// <param name="y">The Y coordinate of the piece</param>
	/// <param name="gamePhase">The game phase</param>
	/// <returns>The evaluation value</returns>
	int evaluationValue(char x, char y, char gamePhase) const override;

};

#endif
