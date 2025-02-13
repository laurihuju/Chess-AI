#include <vector>
#include "rook.h"
#include "../move.h"
#include "../gameState/gameState.h"

/// <summary>
/// The additions and reductions of the value of white rook at different positions.
/// </summary>
int rookValueAdditions[8][8] =
{
	{0, 0, 0, 0, 0, 0, 0, 0},
	{5, 10, 10, 10, 10, 10, 10, 5},
	{-5, 0, 0, 0, 0, 0, 0, -5},
	{-5, 0, 0, 0, 0, 0, 0, -5},
	{-5, 0, 0, 0, 0, 0, 0, -5},
	{-5, 0, 0, 0, 0, 0, 0, -5},
	{-5, 0, 0, 0, 0, 0, 0, -5},
	{0, 0, 0, 5, 5, 0, 0, 0}
};

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

bool Rook::threatensSquare(int ownX, int ownY, int squareX, int squareY, const GameState& gameState) const {
	if (ownX != squareX && ownY != squareY) {
		return false;
	}

	int directionX = squareX > ownX ? 1 : (squareX < ownX ? -1 : 0);
	int directionY = squareY > ownY ? 1 : (squareY < ownY ? -1 : 0);

	int currentX = ownX + directionX;
	int currentY = ownY + directionY;

	while (currentX < 8 && currentX > -1 && currentY < 8 && currentY > -1) {
		if (currentX == squareX && currentY == squareY) {
			return true;
		}
		if (gameState.getPieceAt(currentX, currentY) != 0) {
			return false;
		}

		currentX += directionX;
		currentY += directionY;
	}

	return false;
}

Piece* Rook::clone() const {
	return new Rook(isWhite());
}

int Rook::evaluationValue(const GameState& gameState, int x, int y) const {
	return 500 + rookValueAdditions[isWhite() ? y : 7 - y][x];
}
