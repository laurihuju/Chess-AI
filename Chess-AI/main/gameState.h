#ifndef GAMESTATE_H
#define GAMESTATE_H

class Piece;
class Move;
class King;

class GameState {

private:
	Piece* _board[8][8];

	// Castling
	bool _upperLeftCastlingPossible = true;
	bool _upperRightCastlingPossible = true;
	bool _lowerLeftCastlingPossible = true;
	bool _lowerRightCastlingPossible = true;

	// En passant
	int _upperEnPassantColumn = -1;
	int _lowerEnPassantColumn = -1;

public:
	/// <summary>
	/// Initializes the GameState with default board content.
	/// </summary>
	GameState();

	~GameState();

	/// <summary>
	/// Moves the given move. Handles capture if the move moves a piece to a place where another piece is located.
	/// Does not check if the move is valid!
	/// </summary>
	/// <param name="move"></param>
	void applyMove(const Move& move);

	/// <summary>
	/// Finds the king of the given color. The coordinates of the king are set to the variables x and y.
	/// </summary>
	/// <param name="isWhite"></param>
	/// <param name="x"></param>
	/// <param name="y"></param>
	/// <returns>The pointer to the king, or nullptr if the king was not found.</returns>
	King* findKing(bool isWhite, int& x, int& y) const;

	/// <summary>
	/// Prints the board content to the console.
	/// </summary>
	void printBoard() const;

	/// <summary>
	/// Returns the piece at the given coordinates.
	/// Does not check if the coordinates are valid!
	/// </summary>
	/// <param name="x"></param>
	/// <param name="y"></param>
	/// <returns>The piece at the given coordinates.</returns>
	Piece* getPieceAt(int x, int y) const;

	// Castling
	bool upperLeftCastlingPossible() const;
	bool upperRightCastlingPossible() const;
	bool lowerLeftCastlingPossible() const;
	bool lowerRightCastlingPossible() const;

	// En passant
	int upperEnPassantColumn() const;
	int lowerEnPassantColumn() const;

};

#endif
