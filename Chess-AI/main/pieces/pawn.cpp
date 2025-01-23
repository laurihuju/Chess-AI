#include <vector>
#include "pawn.h"
#include "../move.h"
#include "../gameState.h"

Pawn::Pawn(bool isWhite) : Piece(isWhite) {}

std::vector<Move> Pawn::possibleMoves(int x, int y, const GameState& gameState) const {
	return std::vector<Move>();
}
