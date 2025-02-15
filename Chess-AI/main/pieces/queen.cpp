#include <vector>
#include "queen.h"
#include "../move.h"
#include "../gameState/gameState.h"

/// <summary>
/// The additions and reductions of the value of white queen at different positions.
/// </summary>
int queenValueAdditions[8][8] =
{
	{-20, -10, -10, -5, -5, -10, -10, -20},
	{-10, 0, 0, 0, 0, 0, 0, -10},
	{-10, 0, 5, 5, 5, 5, 0, -10},
	{-5, 0, 5, 5, 5, 5, 0, -5},
	{0, 0, 5, 5, 5, 5, 0, -5},
	{-10, 5, 5, 5, 5, 5, 0, -10},
	{-10, 0, 5, 0, 0, 0, 0, -10},
	{-20, -10, -10, -5, -5, -10, -10, -20}
};

Queen::Queen(bool isWhite) : Bishop(isWhite), Rook(isWhite), Piece(isWhite) {}

PieceType Queen::getType() const {
	return PieceType::Queen;
}

char Queen::gamePhaseInfluence() const {
	return 4;
}

void Queen::possibleMoves(std::vector<Move>& moves, int x, int y, const GameState& gameState) const {
	// Get diagonal moves from Bishop
	Bishop::possibleMoves(moves, x, y, gameState);

	// Get straight moves from Rook
	Rook::possibleMoves(moves, x, y, gameState);
}

bool Queen::threatensSquare(int ownX, int ownY, int squareX, int squareY, const GameState& gameState) const {
	return Bishop::threatensSquare(ownX, ownY, squareX, squareY, gameState) || Rook::threatensSquare(ownX, ownY, squareX, squareY, gameState);
}

int Queen::evaluationValue(const GameState& gameState, int x, int y) const {
	return 900 + queenValueAdditions[isWhite() ? y : 7 - y][x];
}
