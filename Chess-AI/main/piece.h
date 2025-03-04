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
/// The class is immutable, and any child class should be immutable.
/// </summary>
class Piece {

private:
	/// <summary>
	/// Information about if the piece is white.
	/// </summary>
	const bool _isWhite;

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
	/// The influence of this piece to the game phase.
	/// </summary>
	/// <returns></returns>
	virtual char gamePhaseInfluence() const = 0;

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
	virtual void possibleMoves(std::vector<Move>& moves, char x, char y, const GameState& gameState, bool captureOnly = false) const = 0;

	/// <summary>
	/// Checks if this piece at the given location threatens the given square.
	/// </summary>
	/// <param name="ownX"></param>
	/// <param name="ownY"></param>
	/// <param name="squareX"></param>
	/// <param name="squareY"></param>
	/// <param name="gameState"></param>
	/// <returns></returns>
	virtual bool threatensSquare(char ownX, char ownY, char squareX, char squareY, const GameState& gameState) const = 0;

	/// <summary>
	/// The evaluation value of this piece at the given coordinates on the given GameState.
	/// </summary>
	/// <param name="gameState">The game state of evaluation</param>
	/// <param name="x">The X coordinate of the piece</param>
	/// <param name="y">The Y coordinate of the piece</param>
	/// <returns></returns>
	virtual int evaluationValue(const GameState& gameState, char x, char y) const = 0;

};

#endif
