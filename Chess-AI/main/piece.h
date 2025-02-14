#ifndef PIECE_H
#define PIECE_H

#include <vector>

class Move;
class GameState;

/// <summary>
/// Enum class describing piece types.
/// These are used for efficient type checking without RTTI.
/// </summary>
enum class PieceType {
	Bishop,
	King,
	Knight,
	Pawn,
	Queen,
	Rook
};

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
	/// Compares the other piece with this piece.
	/// </summary>
	/// <param name="other"></param>
	/// <returns>True if the pieces are equal</returns>
	bool operator==(const Piece& other) const;

	/// <summary>
	/// Compares the other piece with this piece.
	/// </summary>
	/// <param name="other"></param>
	/// <returns>True if the pieces are not equal</returns>
	bool operator!=(const Piece& other) const;

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
	/// The PieceType of this piece.
	/// </summary>
	/// <returns></returns>
	virtual PieceType getType() const = 0;

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

	/// <summary>
	/// Creates a copy of this piece.
	/// </summary>
	/// <returns>Pointer to the copied piece</returns>
	virtual Piece* clone() const = 0;

	/// <summary>
	/// The evaluation value of this piece at the given coordinates on the given GameState.
	/// </summary>
	/// <param name="gameState">The game state of evaluation</param>
	/// <param name="x">The X coordinate of the piece</param>
	/// <param name="y">The Y coordinate of the piece</param>
	/// <returns></returns>
	virtual int evaluationValue(const GameState& gameState, int x, int y) const = 0;

};

#endif
