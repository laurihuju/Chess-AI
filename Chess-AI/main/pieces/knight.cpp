#include <vector>
#include "knight.h"
#include "../move.h"
#include "../gameState.h"

Knight::Knight(bool isWhite) : Piece(isWhite) {}

std::vector<Move> Knight::possibleMoves(int x, int y, const GameState& gameState) const {
	return std::vector<Move>();
}
