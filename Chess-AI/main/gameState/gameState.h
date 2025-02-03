#ifndef GAMESTATE_H
#define GAMESTATE_H

#include <vector>

class Piece;
class Move;
class King;

class GameState {

protected:
	/// <summary>
	/// The current board content.
	/// </summary>
	Piece* _board[8][8];

	// Castling
	bool _upperLeftCastlingPossible = true;
	bool _upperRightCastlingPossible = true;
	bool _lowerLeftCastlingPossible = true;
	bool _lowerRightCastlingPossible = true;

	// En passant
	int _upperEnPassantColumn = -1;
	int _lowerEnPassantColumn = -1;

	/// <summary>
	/// The pieces this GameState owns and is responsible to destroy.
	/// </summary>
	std::vector<Piece*> ownPieces;

	/// <summary>
	/// Deletes the given piece if it is owned by this GameState.
	/// </summary>
	void handlePieceDelete(Piece* piece);

public:
	GameState& operator=(const GameState&) = delete;
	bool operator==(const GameState& other);

	/// <summary>
	/// Creates a copy of the given GameState. Doesn't copy the vector of owned pieces.
	/// </summary>
	/// <param name="other"></param>
	GameState(const GameState& other);

	/// <summary>
	/// Creates a new GameState with empty board.
	/// </summary>
	GameState();

	/// <summary>
	/// Deletes all pieces owned by this GameState.
	/// </summary>
	virtual ~GameState();

	/// <summary>
	/// Moves the given move. Handles capture if the move moves a piece to a place where another piece is located.
	/// Updates the castling and en passant flags automatically. Handles castling and en passant moves automatically.
	/// Handles piece promotion if the move has specified the promotion piece.
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

	/// <summary>
	/// Returns a vector containing all possible new game states that can be created from this game state with one move of the given color.
	/// </summary>
	/// <param name="isWhite"></param>
	/// <returns></returns>
	std::vector<GameState> possibleNewGameStates(bool isWhite) const;

	/// <summary>
	/// Checks if the king of the given color is in check.
	/// </summary>
	/// <param name="isWhite"></param>
	/// <returns>True if the king of the given color is in check</returns>
	bool isCheck(bool isWhite) const;

	/// <summary>
	/// Checks if the square is threatened by the given opponent.
	/// </summary>
	/// <param name="isWhite"></param>
	/// <param name="x"></param>
	/// <param name="y"></param>
	/// <returns>True if the given square is threatened by the given opponent</returns>
	bool isThreatened(bool isWhite, int x, int y) const;

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
