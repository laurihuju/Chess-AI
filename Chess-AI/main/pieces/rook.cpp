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

PieceType Rook::getType() const {
	return PieceType::Rook;
}

char Rook::gamePhaseInfluence() const {
	return 2;
}

void Rook::possibleMoves(std::vector<Move>& moves, char x, char y, const GameState& gameState, bool captureOnly) const {
	// Directions: up, down, left, right
	char directions[4][2] = { {-1,0}, {1,0}, {0,-1}, {0,1} };
	for (auto& dir : directions) {
		char dx = x;
		char dy = y;
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
			
			if (!captureOnly) {
				moves.push_back(Move(x, y, dx, dy));
			}
		}
	}
}

bool Rook::threatensSquare(char ownX, char ownY, char squareX, char squareY, const GameState& gameState) const {
	if (ownX != squareX && ownY != squareY) {
		return false;
	}

	char directionX = squareX > ownX ? 1 : (squareX < ownX ? -1 : 0);
	char directionY = squareY > ownY ? 1 : (squareY < ownY ? -1 : 0);

	char currentX = ownX + directionX;
	char currentY = ownY + directionY;

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

int Rook::evaluationValue(const GameState& gameState, char x, char y) const {
	return 500 + rookValueAdditions[isWhite() ? y : 7 - y][x];
}
