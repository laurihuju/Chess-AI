#include <vector>
#include <cmath>
#include "knight.h"
#include "../move.h"
#include "../gameState/gameState.h"

Knight::Knight(bool isWhite) : Piece(isWhite) {}

void Knight::possibleMoves(std::vector<Move>& moves, int x, int y, const GameState& gameState) const {
	// Possible moves for a knight
	int directions[8][2] = { {-2, 1}, {-1, 2}, {1, 2}, {2, 1}, {2, -1}, {1, -2}, {-1, -2}, {-2, -1} };
	for (auto& dir : directions) {
		int dx = x + dir[0];
		int dy = y + dir[1];
		if (dx >= 0 && dx <= 7 && dy >= 0 && dy <= 7) {
			Piece* p = gameState.getPieceAt(dx, dy);
			if (!p || p->isWhite() != this->isWhite()) {
				moves.push_back(Move(x, y, dx, dy));
			}
		}
	}

}

bool Knight::threatensSquare(int ownX, int ownY, int squareX, int squareY, const GameState& gameState) const {
	return (std::abs(ownX - squareX) == 2 && std::abs(ownY - squareY) == 1) || std::abs(ownX - squareX) == 1 && std::abs(ownY - squareY) == 2;
}

Piece* Knight::clone() const {
	return new Knight(isWhite());
}

int Knight::evaluationValue(const GameState& gameState, int x, int y) const {
	return 3;
}
