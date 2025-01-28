#include <vector>
#include "king.h"
#include "../move.h"
#include "../gameState/gameState.h"

King::King(bool isWhite) : Piece(isWhite) {}

void King::possibleMoves(std::vector<Move>& moves, int x, int y, const GameState& gameState) const {
	// Directions: up, down, left, right
	int directions[8][2] = { { -1,0 },{ 1,0 },{ 0,-1 },{ 0,1 },{ -1,-1 },{ -1,1 },{ 1,-1 },{ 1,1 } };
	for (auto& dir : directions) {
		int dx = x + dir[0];
		int dy = y + dir[1];
		if (dx < 0 || dx > 7 || dy < 0 || dy > 7)
			continue;
		Piece* p = gameState.getPieceAt(dx, dy);
		if (p) {
			if (p->isWhite() != this->isWhite()) {
				moves.push_back(Move(x, y, dx, dy));
			}
		}
		else {
			moves.push_back(Move(x, y, dx, dy));
		}
	}
}

/*for (int i = (x - 1 >= 0 ? x - 1 : x); i <= x + 1 && i < 8; i++) {
		for (int j = (y - 1 >= 0 ? y - 1 : y); i <= y + 1 && j < 8; j++) {
			if (i == x && j == y)
				continue;

			Piece* squarePiece = gameState.getPieceAt(x, y);
			if (squarePiece != 0 && squarePiece->isWhite() == isWhite())
				continue;

			moves.push_back(Move(x, y, i, j));
		}
	}*/