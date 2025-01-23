#include <vector>
#include "king.h"
#include "../move.h"
#include "../gameState.h"

King::King(bool isWhite) : Piece(isWhite) {}

std::vector<Move> King::possibleMoves(int x, int y, const GameState& gameState) const {
	return std::vector<Move>();
}
