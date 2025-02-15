#ifndef MOVE_H
#define MOVE_H

#include <string>
#include "move.h"

/// <summary>
/// Describes move from one square to another.
/// Can also contain optional information about piece promotion.
/// </summary>
class Move {

private:
	/// <summary>
	/// The 'from' X coordinate of this move.
	/// The coordinate is stored as internal index coordinate.
	/// </summary>
	int _x1;

	/// <summary>
	/// The 'from' Y coordinate of this move.
	/// The coordinate is stored as internal index coordinate.
	/// </summary>
	int _y1;

	/// <summary>
	/// The 'to' X coordinate of this move.
	/// The coordinate is stored as internal index coordinate.
	/// </summary>
	int _x2;

	/// <summary>
	/// The 'to' Y coordinate of this move.
	/// The coordinate is stored as internal index coordinate.
	/// </summary>
	int _y2;

	/// <summary>
	/// The piece this move promotes to.
	/// ('q' = queen; 'n' = knight; 'b' = bishop; 'r' = rook; -1 = no promotion)
	/// </summary>
	char _promotionPiece;

	/// <summary>
	/// Converts the given display X coordinate char to internal index coordinate.
	/// </summary>
	/// <param name="coordinateInputChar">The display coordinate char</param>
	/// <returns>The internal index coordinate</returns>
	int convertXCoordinateFromInput(char coordinateInputChar) const;

	/// <summary>
	/// Converts the given display X coordinate char to internal index coordinate.
	/// </summary>
	/// <param name="coordinateInputChar">The display coordinate char</param>
	/// <returns>The internal index coordinate</returns>
	int convertYCoordinateFromInput(char coordinateInputChar) const;

public:
	/// <summary>
	/// Checks if the other move is the same as this move.
	/// </summary>
	/// <param name="other">The other move</param>
	/// <returns>True if the other move is the same as this move</returns>
	bool operator==(const Move& other) const;

	/// <summary>
	/// Checks if the other move is the same as this move.
	/// </summary>
	/// <param name="other">The other move</param>
	/// <returns>True if the other move is not the same as this move</returns>
	bool operator!=(const Move& other) const;

	/// <summary>
	/// Creates a new move with 'from' and 'to' coordinates.
	/// All coordinates are given as internal index coordinates.
	/// </summary>
	/// <param name="x1">The 'from' X coordinate</param>
	/// <param name="y1">The 'from' Y coordinate</param>
	/// <param name="x2">The 'to' X coordinate</param>
	/// <param name="y2">The 'to' Y coordinate</param>
	Move(int x1, int y1, int x2, int y2);

	/// <summary>
	/// Creates a new move with 'from' and 'to' coordinates and promotion.
	/// All coordinates are given as internal index coordinates.
	/// The promotionPiece char defines which promotion is done ('q' = queen; 'n' = knight; 'b' = bishop; 'r' = rook; -1 = no promotion).
	/// </summary>
	/// <param name="x1">The 'from' X coordinate</param>
	/// <param name="y1">The 'from' Y coordinate</param>
	/// <param name="x2">The 'to' X coordinate</param>
	/// <param name="y2">The 'to' Y coordinate</param>
	/// <param name="promotionPiece">The char of the promotion piece (see summary)</param>
	Move(int x1, int y1, int x2, int y2, char promotionPiece);

	/// <summary>
	/// Creates a new move from string input.
	/// All coordinates in the string input are given as display coordinates, not internal coordinates.
	/// The string input is given in the following format: "&lt;x1&gt;&lt;y1&gt;&lt;x2&gt;&lt;y2&gt;[promotionPiece]".
	/// The promotionPiece defines which promotion is done ('q' = queen; 'n' = knight; 'b' = bishop; 'r' = rook).
	/// Promotes to queen if promotionPiece is not valid.
	/// </summary>
	/// <param name="input">The input string</param>
	Move(const std::string& input);

	/// <summary>
	/// The 'from' X coordinate of this move.
	/// The coordinate is given as internal index coordinate.
	/// </summary>
	/// <returns>The 'from' X coordinate of this move.</returns>
	int x1() const;

	/// <summary>
	/// The 'from' Y coordinate of this move.
	/// The coordinate is given as internal index coordinate.
	/// </summary>
	/// <returns>The 'from' Y coordinate of this move.</returns>
	int y1() const;

	/// <summary>
	/// The 'to' X coordinate of this move.
	/// The coordinate is given as internal index coordinate.
	/// </summary>
	/// <returns>The 'to' X coordinate of this move.</returns>
	int x2() const;

	/// <summary>
	/// The 'to' Y coordinate of this move.
	/// The coordinate is given as internal index coordinate.
	/// </summary>
	/// <returns>The 'to' Y coordinate of this move</returns>
	int y2() const;

	/// <summary>
	/// The promotion piece of the move. ('q' = queen; 'n' = knight; 'b' = bishop; 'r' = rook; -1 = no promotion)
	/// </summary>
	/// <returns></returns>
	char promotionPiece() const;

};

#endif
