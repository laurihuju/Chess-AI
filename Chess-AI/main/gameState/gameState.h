#ifndef GAMESTATE_H
#define GAMESTATE_H

#include <vector>

class Piece;
class Move;
class King;

/// <summary>
/// Describes a game state.
/// </summary>
class GameState {

protected:
	/// <summary>
	/// The current board content.
	/// </summary>
	Piece* _board[8][8];

	/// <summary>
	/// Flag for checking if upper left castling is still possible during the game.
	/// </summary>
	bool _upperLeftCastlingPossible = true;

	/// <summary>
	/// Flag for checking if upper right castling is still possible during the game.
	/// </summary>
	bool _upperRightCastlingPossible = true;

	/// <summary>
	/// Flag for checking if lower left castling is still possible during the game.
	/// </summary>
	bool _lowerLeftCastlingPossible = true;

	/// <summary>
	/// Flag for checking if lower right castling is still possible during the game.
	/// </summary>
	bool _lowerRightCastlingPossible = true;

	/// <summary>
	/// The column which upper en passant is possible to do to (the column of the piece that would be captured in
	/// the en passant move). Has value -1 if upper en passant is not possible.
	/// </summary>
	int _upperEnPassantColumn = -1;

	/// <summary>
	/// The column which lower en passant is possible to do to (the column of the piece that would be captured in
	/// the en passant move). Has value -1 if lower en passant is not possible.
	/// </summary>
	int _lowerEnPassantColumn = -1;

	/// <summary>
	/// The game phase value which is sum of the game phase influence value of all pieces on the board.
	/// </summary>
	char _gamePhase;

public:
	GameState& operator=(const GameState&) = delete;

	/// <summary>
	/// Compares the other game state with this game state.
	/// </summary>
	/// <param name="other"></param>
	/// <returns>True if the game states are equal</returns>
	bool operator==(const GameState& other) const;

	/// <summary>
	/// Compares the other game state with this game state.
	/// </summary>
	/// <param name="other"></param>
	/// <returns>True if the game states are not equal</returns>
	bool operator!=(const GameState& other) const;

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
	/// Handles piece promotion if the move has specified the promotion piece (the promotion piece instance will be searched from the CurrentGameState instance).
	/// Does not check if the move is valid!
	/// </summary>
	/// <param name="move">The move to apply</param>
	void applyMove(const Move& move);

	/// <summary>
	/// Finds the king of the given color. The coordinates of the king are set to the variables x and y.
	/// </summary>
	/// <param name="isWhite">Information about if the king to find is white</param>
	/// <param name="x">The variable where the X coordinate of the king will be set</param>
	/// <param name="y">The variable where the Y coordinate of the king will be set</param>
	void findKing(bool isWhite, int& x, int& y) const;

	/// <summary>
	/// Prints the board content to the console.
	/// </summary>
	void printBoard() const;

	/// <summary>
	/// Returns the piece at the given coordinates.
	/// The coordinates are given as internal index coordinates!
	/// Does not check if the coordinates are valid!
	/// </summary>
	/// <param name="x">The x coordinate of the piece</param>
	/// <param name="y">The y coordinate of the piece</param>
	/// <returns>The piece at the given coordinates.</returns>
	Piece* getPieceAt(int x, int y) const;

	/// <summary>
	/// Returns a vector containing all possible new game states that can be created
	/// from this game state with one move of the given color.
	/// </summary>
	/// <param name="isWhite">If the new game states should be generated for moves of white</param>
	/// <returns>A vector containing all possible new game states with one move of the given color</returns>
	

	/// <summary>
	/// Generates all possible new game states that can be created from this game state with one
	/// move of the given color, and adds them to the newGameStates vector.
	/// </summary>
	/// <param name="newGameStates">The vector where the new game states will be added</param>
	/// <param name="isWhite">If the new game states should be generated for moves of white</param>
	void possibleNewGameStates(std::vector<GameState>& newGameStates, bool isWhite) const;

	/// <summary>
	/// Checks if the king of the given color is in check.
	/// </summary>
	/// <param name="isWhite">If the king to check is white</param>
	/// <returns>True if the king of the given color is in check</returns>
	bool isCheck(bool isWhite) const;

	/// <summary>
	/// Checks if the square is threatened by the opponent at the perspective of the given color.
	/// The coordinates are given as internal index coordinates!
	/// </summary>
	/// <param name="isWhite">If to theck from the perspective of white</param>
	/// <param name="x">The X coordinate of the square</param>
	/// <param name="y">The Y coordinate of the square</param>
	/// <returns>True if the given square is threatened</returns>
	bool isThreatened(bool isWhite, int x, int y) const;

	/// <summary>
	/// Evaluates this GameState for the given color.
	/// Higher evaluation value means this game state is better for the given color
	/// and lower means this game state is worse for the given color.
	/// </summary>
	/// <param>If the function should evaluate from the perspective of white</param>
	/// <returns>The evaluation value</returns>
	int evaluate(bool isWhite) const;

	/// <summary>
	/// Checks if upper left castling is still possible in this game state.
	/// Does not check if there is space for castling or if the squares are threatened.
	/// </summary>
	/// <returns>True if upper left castling is possible</returns>
	bool upperLeftCastlingPossible() const;

	/// <summary>
	/// Checks if upper right castling is still possible in this game state.
	/// Does not check if there is space for castling or if the squares are threatened.
	/// </summary>
	/// <returns>True if upper right castling is possible</returns>
	bool upperRightCastlingPossible() const;

	/// <summary>
	/// Checks if lower left castling is still possible in this game state.
	/// Does not check if there is space for castling or if the squares are threatened.
	/// </summary>
	/// <returns>True if lower left castling is possible</returns>
	bool lowerLeftCastlingPossible() const;

	/// <summary>
	/// Checks if lower right castling is still possible in this game state.
	/// Does not check if there is space for castling or if the squares are threatened.
	/// </summary>
	/// <returns>True if lower right castling is possible</returns>
	bool lowerRightCastlingPossible() const;

	/// <summary>
	/// Checks which column upper en passant move is possible to do at.
	/// The returned column is the column of the piece that would be captured in the en passant move.
	/// Returns -1 if upper en passant is not possible.
	/// </summary>
	/// <returns>The column where upper en passant move is possible to do at, or -1 if upper en passant is not possible</returns>
	int upperEnPassantColumn() const;

	/// <summary>
	/// Checks which column lower en passant move is possible to do at.
	/// The returned column is the column of the piece that would be captured in the en passant move.
	/// Returns -1 if lower en passant is not possible.
	/// </summary>
	/// <returns>The column where lower en passant move is possible to do at, or -1 if lower en passant is not possible</returns>
	int lowerEnPassantColumn() const;

	/// <summary>
	/// The game phase value which is sum of the game phase influence value of all pieces on the board.
	/// </summary>
	/// <returns></returns>
	char gamePhase() const;

};

#endif
