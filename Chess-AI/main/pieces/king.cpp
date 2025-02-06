#include <vector>
#include <cmath>
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

	// Castling
	if (!gameState.isCheck(isWhite())) {
		int castlingRow = isWhite() ? 7 : 0;

		// Left castling
		if ((isWhite() ? gameState.lowerLeftCastlingPossible() : gameState.upperLeftCastlingPossible()) && gameState.getPieceAt(1, castlingRow) == 0 && gameState.getPieceAt(2, castlingRow) == 0 && gameState.getPieceAt(3, castlingRow) == 0 && !gameState.isThreatened(isWhite(), 2, castlingRow) && !gameState.isThreatened(isWhite(), 3, castlingRow)) {
			moves.push_back(Move(4, 7, 2, 7));
		}

		// Right castling
		if ((isWhite() ? gameState.lowerRightCastlingPossible() : gameState.upperRightCastlingPossible()) && gameState.getPieceAt(5, castlingRow) == 0 && gameState.getPieceAt(6, castlingRow) == 0 && !gameState.isThreatened(true, 5, castlingRow) && !gameState.isThreatened(isWhite(), 6, castlingRow)) {
			moves.push_back(Move(4, 7, 6, 7));
		}
	}

}

bool King::threatensSquare(int ownX, int ownY, int squareX, int squareY, const GameState& gameState) const {
	return std::abs(ownX - squareX) <= 1 && std::abs(ownY - squareY) <= 1;
}

Piece* King::clone() const {
	return new King(isWhite());
}
