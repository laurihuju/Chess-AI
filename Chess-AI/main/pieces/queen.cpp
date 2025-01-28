#include <vector>
#include "queen.h"
#include "../move.h"
#include "../gameState.h"

Queen::Queen(bool isWhite) : Bishop(isWhite), Rook(isWhite), Piece(isWhite) {}

void Queen::possibleMoves(std::vector<Move>& moves, int x, int y, const GameState& gameState) const {
	// Get diagonal moves from Bishop
	Bishop::possibleMoves(moves, x, y, gameState);

	// Get straight moves from Rook
	Rook::possibleMoves(moves, x, y, gameState);
}
