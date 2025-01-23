#include <vector>
#include "queen.h"
#include "../move.h"
#include "../gameState.h"

Queen::Queen(bool isWhite) : Piece(isWhite) {}

std::vector<Move> Queen::possibleMoves(int x, int y, const GameState& gameState) const {
	return std::vector<Move>();
}
