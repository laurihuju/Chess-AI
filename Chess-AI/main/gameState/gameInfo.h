#ifndef GAMEINFO_H
#define GAMEINFO_H

#include <vector>
#include <random>
#include "../piece.h"

/// <summary>
/// Class that contains chess game info like piece instances and zobrist values for flags.
/// </summary>
class GameInfo {

private:
	/// <summary>
	/// The signeton instance of GameInfo.
	/// </summary>
	static GameInfo* instance;

	/// <summary>
	/// All pieces created by the current game state.
	/// These are deleted when the current game state gets deleted.
	/// </summary>
	std::vector<Piece*> _pieces;

	/// <summary>
	/// Zobrist hashing values for different pieces at different squares.
	/// </summary>
	uint64_t _pieceZobristValues[12][8][8];

	/// <summary>
	/// Zobrist hashing value to be used if the side to move is white.
	/// </summary>
	uint64_t _whiteSideToMoveZobristValue;

	/// <summary>
	/// Zobrist hashing value for upper left castling possibility.
	/// </summary>
	uint64_t _upperLeftCastlingZobristValue;

	/// <summary>
	/// Zobrist hashing value for upper right castling possibility.
	/// </summary>
	uint64_t _upperRightCastlingZobristValue;

	/// <summary>
	/// Zobrist hashing value for lower left castling possibility.
	/// </summary>
	uint64_t _lowerLeftCastlingZobristValue;

	/// <summary>
	/// Zobrist hashing value for lower right castling possibility.
	/// </summary>
	uint64_t _lowerRightCastlingZobristValue;

	/// <summary>
	/// Zobrist hashing values for upper en passant columns.
	/// </summary>
	uint64_t _upperEnPassantZobristValues[8];

	/// <summary>
	/// Zobrist hashing values for lower en passant columns.
	/// </summary>
	uint64_t _lowerEnPassantZobristValues[8];

public:
	/// <summary>
	/// Creates new GameInfo.
	/// </summary>
	GameInfo();

	/// <summary>
	/// Deletes all pieces created by this GameInfo.
	/// </summary>
	~GameInfo();

	/// <summary>
	/// Searches the given type and color piece from the piece instances
	/// created by this game info.
	/// </summary>
	/// <param name="type">The piece type to search</param>
	/// <param name="isWhite">If to search white piece</param>
	/// <returns>The found piece, or nullptr if not found</returns>
	Piece* getPieceInstance(PieceType type, bool isWhite) const;

	/// <summary>
	/// Zobrist hashing value for the given piece type and color at the given square.
	/// </summary>
	/// <param name="type">The piece type</param>
	/// <param name="isWhite">If the piece is white</param>
	/// <param name="x">The X coordinate of the square</param>
	/// <param name="y">The Y coordinate of the square</param>
	/// <returns>The zobrist value of the given piece type at the given square</returns>
	uint64_t pieceZobristValue(PieceType type, bool isWhite, char x, char y);

	/// <summary>
	/// 
	/// </summary>
	/// <returns></returns>
	uint64_t whiteSideToMoveZobristValue() const;
	/// <summary>
	/// Zobrist hashing value for upper left castling possibility.
	/// </summary>
	/// <returns>The zobrist value</returns>
	uint64_t upperLeftCastlingZobristValue() const;

	/// <summary>
	/// Zobrist hashing value for upper right castling possibility.
	/// </summary>
	/// <returns>The zobrist value</returns>
	uint64_t upperRightCastlingZobristValue() const;

	/// <summary>
	/// Zobrist hashing value for lower left castling possibility.
	/// </summary>
	/// <returns>The zobrist value</returns>
	uint64_t lowerLeftCastlingZobristValue() const;

	/// <summary>
	/// Zobrist hashing value for lower right castling possibility.
	/// </summary>
	/// <returns>The zobrist value</returns>
	uint64_t lowerRightCastlingZobristValue() const;

	/// <summary>
	/// Zobrist hashing value for upper en passant column of the given X coordinate.
	/// </summary>
	/// <param name="x">The X coordinate for zobrist value</param>
	/// <returns>The zobrist value</returns>
	uint64_t upperEnPassantZobristValue(int x) const;

	/// <summary>
	/// Zobrist hashing value for lower en passant column of the given X coordinate.
	/// </summary>
	/// <param name="x">The X coordinate for zobrist value</param>
	/// <returns>The zobrist value</returns>
	uint64_t lowerEnPassantZobristValue(int x) const;

	/// <summary>
	/// Gets the singleton instance of GameInfo.
	/// </summary>
	/// <returns>The singleton instance</returns>
	static GameInfo* getInstance();

};

#endif
