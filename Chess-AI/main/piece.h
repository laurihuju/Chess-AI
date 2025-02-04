#ifndef PIECE_H
#define PIECE_H

#include <vector>

class Move;
class GameState;

/// <summary>
/// An abstract class that describes a basic chess piece with only color.
/// </summary>
class Piece {

private:
	/// <summary>
	/// Information about if the piece is white.
	/// </summary>
	bool _isWhite;

public:
	/// <summary>
	/// Initializes the piece.
	/// </summary>
	/// <param name="isWhite">If the piece is white</param>
	Piece(bool isWhite);

	/// <summary>
	/// Destroys the piece.
	/// </summary>
	virtual ~Piece();

	/// <summary>
	/// Information about if the piece is white.
	/// </summary>
	/// <returns>True if the piece is white</returns>
	bool isWhite() const;

	/// <summary>
	/// Adds all the possible moves of this piece to the moves vector. Does not take into account if the king is threatened.
	/// </summary>
	/// <param name="moves"></param>
	/// <param name="x">The x coordinate of this piece</param>
	/// <param name="y">The y coordinate of this piece</param>
	/// <param name="gameState">The game state from which the possible moves are generated</param>
	virtual void possibleMoves(std::vector<Move>& moves, int x, int y, const GameState& gameState) const = 0;

	/// <summary>
	/// Checks if this piece at the given location threatens the given square.
	/// </summary>
	/// <param name="ownX"></param>
	/// <param name="ownY"></param>
	/// <param name="squareX"></param>
	/// <param name="squareY"></param>
	/// <param name="gameState"></param>
	/// <returns></returns>
	virtual bool threatensSquare(int ownX, int ownY, int squareX, int squareY, const GameState& gameState) const = 0;

};

#endif
