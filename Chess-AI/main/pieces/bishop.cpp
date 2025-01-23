#include <vector>
#include "bishop.h"
#include "../move.h"
#include "../gameState.h"

Bishop::Bishop(bool isWhite) : Piece(isWhite) {}

std::vector<Move> Bishop::possibleMoves(int x, int y, const GameState& gameState) const {
	return std::vector<Move>();
}
