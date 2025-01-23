#include <vector>
#include "rook.h"
#include "../move.h"
#include "../gameState.h"

Rook::Rook(bool isWhite) : Piece(isWhite) {}

std::vector<Move> Rook::possibleMoves(int x, int y, const GameState& gameState) const {
	return std::vector<Move>();
}
