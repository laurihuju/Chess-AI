#include <vector>
#include "rook.h"
#include "../move.h"
#include "../gameState/gameState.h"

Rook::Rook(bool isWhite) : Piece(isWhite) {}

void Rook::possibleMoves(std::vector<Move>& moves, int x, int y, const GameState& gameState) const {
	// Directions: up, down, left, right
	int directions[4][2] = { {-1,0}, {1,0}, {0,-1}, {0,1} };
	for (auto& dir : directions) {
		int dx = x;
		int dy = y;
		while (true) {
			dx += dir[0];
			dy += dir[1];
			if (dx < 0 || dx > 7 || dy < 0 || dy > 7)
				break;

			Piece* p = gameState.getPieceAt(dx, dy);
			if (p) {
				if (p->isWhite() != this->isWhite()) {
					moves.push_back(Move(x, y, dx, dy));
				}
				break;
			}
			
			moves.push_back(Move(x, y, dx, dy));
		}
	}
}
