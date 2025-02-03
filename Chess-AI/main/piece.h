#ifndef PIECE_H
#define PIECE_H

#include <vector>

class Move;
class GameState;

class Piece {

private:
	bool _isWhite;

public:
	Piece(bool isWhite);
	virtual ~Piece();

	bool isWhite() const;

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
